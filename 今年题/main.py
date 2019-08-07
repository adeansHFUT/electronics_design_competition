#8月 7 2019

#如果要用彩图模式,请参考openmv自带的历程
#软件修正摄像头歪，＋22
import sensor, image, time
from pyb import UART
from pyb import LED
import pyb
import json

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) #
sensor.set_framesize(sensor.QVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
#sensor.set_hmirror(True)#水平翻转
#sensor.set_vflip(True)#竖直反转
clock = time.clock() # Tracks FPS.
uart = UART(3, 115200)#串口配

led = pyb.LED(1) # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
led.on()
time.sleep(150)
led.off()
time.sleep(100)
led.on()
time.sleep(150)
led.off()
time.sleep(600)

def find_max(blobs):#寻找最大色块的函数
    min_size=2 #把小于50面积的色块滤掉,如果小球面积本身就很小,则需要把这个值调小一点
    max_size=500 #最大像素
    max_w = 35  # 色块最大宽度
    min_w = 2
    max_blob = 0
    for blob in blobs:
        if ((blob.pixels() < max_size)and(blob.pixels() > min_size)and(blob.w()<max_w)and(blob.h()<max_w)and(blob.w()>min_w)and(blob.h()>min_w)):
        #对识别的色块进行限制,小球在摄像头中的长宽应该要大于5,小于26
            if(max_blob != 0):
                if(blob.pixels()>max_blob.pixels()):
                    max_blob = blob
                    max_size = blob.pixels()
            else:
                max_blob=blob
                max_size = blob.pixels()

    return max_blob

K=2285 #the value should be measured (距离系数)
red=(50, 80, 42, 103, -3, 95)  #红色的LAB阈值
x1 = (11, 54)  # 左上角
board_width = 230
board_high = 165
left_roi = [x1[0], x1[1], board_width, board_high]#感兴趣区
amend = 22



while(True):
    clock.tick()
    img = sensor.snapshot() # Take a picture and return the image.
    blobs = img.find_blobs([red],roi=left_roi, merge=True, margin=10)#参数为 阈值,识别区域
    #for blob in blobs:
       # img.draw_rectangle(blob.rect())#画方框

    if blobs:
        max_blob=find_max(blobs)
        if max_blob!=0:
            img.draw_rectangle(max_blob.rect(), color = (0,255,0))#画方框
            x_ban=max_blob.cx() + amend #求色块中心x坐标
            data = bytearray([0xAA,0x55,x_ban,0x54])#转成16进制
            #如果识别的坐标大于255，建议除以2之后再发，因为一个字节范围只有0-255
            #time.sleep(50)
            print("发送X坐标:"+str(x_ban))
            uart.write(data)#通过串口发送出去数据
            #Lm = (max_blob[2]+max_blob[3])/2
            #length = K/Lm
            #print("距离："+str(length))
    else:
        print('not found!')
    print(clock.fps())#帧率


