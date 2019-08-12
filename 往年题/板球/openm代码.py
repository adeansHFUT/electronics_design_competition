# Untitled - By: Administrator - 周一 4月 15 2019
# 使用的灰度图模式  从黑板识别白球
#如果要用彩图模式,请参考openmv自带的历程
import sensor, image, time
from pyb import UART
from pyb import LED
import json

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.GRAYSCALE) #
sensor.set_framesize(sensor.QVGA) # use QQVGA for speed.
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
#sensor.set_hmirror(True)#水平翻转
#sensor.set_vflip(True)#竖直反转
clock = time.clock() # Tracks FPS.

uart = UART(3, 115200)#串口配置
def find_max(blobs):#寻找最大色块的函数
    min_size=2 #把小于50面积的色块滤掉,如果小球面积本身就很小,则需要把这个值调小一点
    max_size=100 #最大像素
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


white=(225, 255)#银色小球和蓝底 阈值
x1 = (45, 42)  # 左上角
board_width = 187
board_high = 177
left_roi = [x1[0], x1[1], board_width, board_high]#板子区域 摄像头高度33cm
while(True):
    clock.tick()
    img = sensor.snapshot() # Take a picture and return the image.
    blobs = img.find_blobs([white],roi=left_roi, merge=True, margin=10)#参数为 阈值,识别区域
    #for blob in blobs:
       # img.draw_rectangle(blob.rect())#画方框

    if blobs:
        max_blob=find_max(blobs)
        if max_blob!=0:
            img.draw_rectangle(max_blob.rect(), color = (255,0,0))#画方框
            x_ban=max_blob.cx()-x1[0]#求色块中心相对于板子的坐标
            y_ban=max_blob.cy()-x1[1]#求色块中心相对于板子的坐标
            if x_ban<0:
                x_ban=0
            if x_ban>200:
                x_ban=200
            if y_ban<0:
                y_ban=0
            if y_ban>200:
                y_ban=200
            #对坐标进行限幅,因为板子像素就在这范围内。
            checkout=(x_ban+y_ban)#校验位,为x坐标+y坐标 的低八位
            data = bytearray([0xAA,0x55,x_ban,y_ban,checkout,0x54])#转成16进制
            #如果识别的坐标大于255，建议除以2之后再发，因为一个字节范围只有0-255
            print("发送坐标:"+str(x_ban)+ ","+str(y_ban))
            uart.write(data)#通过串口发送出去数据
    else:
        print('not found!')
    print(clock.fps())#帧率


