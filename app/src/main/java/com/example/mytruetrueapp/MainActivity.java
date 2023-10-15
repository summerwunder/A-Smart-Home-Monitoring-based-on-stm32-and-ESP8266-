package com.example.mytruetrueapp;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Handler;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;


public class MainActivity extends AppCompatActivity {

    //private final String host="tcp://broker.emqx.io:1883";
    //private final String host="tcp://10.79.217.111:1883";
    private final String host="tcp://192.168.43.222:1883";
    private final String userName = "android";
    private final String passWord = "android";
    private final String mqtt_id = "2405499352";
    private final String mqtt_sub_topic = "esp/#";
    private final String mqtt_pub_topic = "led/1";
    private MqttClient client;
    private MqttConnectOptions options;
    //主线程
    private Handler handler;

    private ImageView led;
    private TextView temp;
    private TextView humi;
    private ScheduledExecutorService scheduler;

    private boolean led_on=false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        UI_init();
        Mqtt_init();
        startReconnect();

        handler=new Handler(Looper.myLooper()) {
            @SuppressLint({"SetTextIl8n", "HandlerLeak"})
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what){
                    case 1: //开机校验更新回传
                        break;
                    case 2:  // 反馈回传

                        break;
                    case 3:  //MQTT 收到消息回传   UTF8Buffer msg=new UTF8Buffer(object.toString());
                        //Toast.makeText(MainActivity.this,msg.obj.toString() ,Toast.LENGTH_SHORT).show();
                        
                        String str=msg.obj.toString();
                        String jsonData="";
                        try {
                            String[] parts = str.split("---");
                            if (parts.length == 2) {
                                jsonData = parts[1].trim();
                                // 创建JSONObject实例并传入JSON字符串
                            }
                            JSONObject json = new JSONObject(jsonData);

                            // 从JSONObject中获取"humi"和"temp"的值
                            int humiValue = json.getInt("humi");
                            int tempValue = json.getInt("temp");

                            temp.setText(tempValue+"C");
                            humi.setText(humiValue+"%");
                        } catch (JSONException e) {
                            // 处理JSON解析异常
                            e.printStackTrace();
                        }

                        break;
                    case 30:  //连接失败
                        Toast.makeText(MainActivity.this,"连接失败" ,Toast.LENGTH_SHORT).show();
                        break;
                    case 31:   //连接成功
                        Toast.makeText(MainActivity.this,"连接成功" ,Toast.LENGTH_SHORT).show();
                        try {
                            client.subscribe(mqtt_sub_topic,1);
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                        break;
                    default:
                        break;
                }
            }
        };
    }

    /*

        用于初始化MQTT
     */
    private void Mqtt_init()
    {
        try {
            //host为主机名，test为clientid即连接MQTT的客户端ID，一般以客户端唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(host, mqtt_id,new MemoryPersistence());
            //MQTT的连接设置
            options = new MqttConnectOptions();
            //设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，这里设置为true表示每次连接到服务器都以新的身份连接
            options.setCleanSession(false);
            //设置连接的用户名
            options.setUserName(userName);
            //设置连接的密码
            options.setPassword(passWord.toCharArray());
            // 设置超时时间 单位为秒
            options.setConnectionTimeout(10);
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options.setKeepAliveInterval(20);
            //设置回调
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //连接丢失后，一般在这里面进行重连
                    System.out.println("connectionLost----------");
                    startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish后会执行到这里
                    System.out.println("deliveryComplete---------" + token.isComplete());
                    Toast.makeText(MainActivity.this,"发送信息",Toast.LENGTH_SHORT).show();
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message)
                        throws Exception {
                    //subscribe后得到的消息会执行到这里面
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //收到消息标志位
                    msg.obj = topicName + "---" + message.toString();
                    handler.sendMessage(msg);    // hander 回传
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void Mqtt_connect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    if(!(client.isConnected()) )  //如果还未连接
                    {
                        client.connect(options);
                        Message msg = new Message();
                        msg.what = 31;
                        handler.sendMessage(msg);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    Message msg = new Message();
                    msg.what = 30;
                    handler.sendMessage(msg);
                }
            }
        }).start();
    }
    //自动重连定时器
    private void startReconnect() {
        //创建单线程的定时执行器
        scheduler = Executors.newSingleThreadScheduledExecutor();
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                if (!client.isConnected()) {
                    Mqtt_connect();
                }
            }
        }, 0 * 1000, 10 * 1000, TimeUnit.MILLISECONDS);//任务将在启动后立即执行，然后每隔 10 秒执行一次
    }
    private void publishMessage(String topic, String msg)
    {
        if (client == null || !client.isConnected()) {
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setPayload(msg.getBytes());
        try {
            client.publish(topic,message);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void UI_init()
    {
        led=findViewById(R.id.btn_led);
        //监听led点击时间
        led.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(led_on)
                {
                    led.setImageResource(R.mipmap.led_off);
                    publishMessage(mqtt_pub_topic,"LED_OFF");
                    led_on=false;
                }else{
                    led.setImageResource(R.mipmap.led_on);
                    publishMessage(mqtt_pub_topic,"LED_ON");
                    led_on=true;
                }

            }
        });

        temp=findViewById(R.id.temp_1);
        humi=findViewById(R.id.humi_1);
    }
}