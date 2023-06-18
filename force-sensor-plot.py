import serial
import matplotlib.pyplot as plt
from drawnow import drawnow

forceX = []
forceY = []
forceZ = []
arduinoData = serial.Serial('COM6', 115200)
plt.ion()

def makeFig():
    plt.ylim(-5, 5)
    plt.title('Force Sensor Data')
    plt.grid(True)
    plt.ylabel('Force')
    plt.plot(forceX, 'ro-', label='Force X')
    plt.plot(forceY, 'go-', label='Force Y')
    plt.plot(forceZ, 'bo-', label='Force Z')
    plt.legend(loc='upper left')

while True:
    while arduinoData.inWaiting() == 0:
        pass
    arduinoString = arduinoData.readline().decode().strip()
    dataArray = arduinoString.split(',')
    if len(dataArray) != 3:
        continue
    try:
        fx = float(dataArray[0])
        fy = float(dataArray[1])
        fz = float(dataArray[2])
        forceX.append(fx)
        forceY.append(fy)
        forceZ.append(fz)
        drawnow(makeFig)
        plt.pause(0.000001)
        if len(forceX) > 50:
            forceX.pop(0)
            forceY.pop(0)
            forceZ.pop(0)
    except ValueError:
        continue