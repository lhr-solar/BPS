from matrix import Matrix
import math
import matplotlib.pyplot as plt
import random

#requires Matrix library, in the linear algebra repository on my github!
class KalmanFilter():

    def __init__(self,stateMatrix, stateErrors, transitionMatrix, measureMatrix, measureErrors, externalTransition = None, externalMotion = None): #all we do is assign the respective matrices to our object variables, nothing special
        self.stateMatrix = stateMatrix
        self.stateErrors = stateErrors 
        self.transitionMatrix = transitionMatrix

        self.measureMatrix = measureMatrix
        self.measureErrors = measureErrors

        self.externalTransition = externalTransition #our control command transition matrix, similar to how the state transition matrix works
        self.externalMotion = externalMotion #our control commands
    
    def calcKalmanGain(self):
        print("\nState Errors:")
        print(self.stateErrors)
        print()
        totalError = self.measureMatrix * self.stateErrors * self.measureMatrix.transpose() + self.measureErrors
        print("\nMultiplying by measure Matrix:")
        print(self.measureMatrix * self.stateErrors * self.measureMatrix.transpose())
        print()
        print("\nMeasurement Errors:")
        print(self.measureErrors)
        print()
        print("\nTotal Error:")
        print(totalError)
        print()
        print("\nState errors by measure: ")
        print(self.stateErrors * self.measureMatrix.transpose())
        print()
        return self.stateErrors * self.measureMatrix.transpose() * totalError.inverse() 

    def updateError(self, gain):              #updates erorr after a measurement, we get more accuarate after each measurement
        self.stateErrors = (Matrix.identity(2) - gain * self.measureMatrix) * self.stateErrors
        print("\nNew coeeficient: ")
        print(Matrix.identity(2) - gain * self.measureMatrix)
        print()
        print("\nNew errors:")
        print(self.stateErrors)
        print()
        
    def updateEstimate(self, measurement):          #uses a measurment to refine its estimate
        z = Matrix([measurement])
        y = z - self.measureMatrix * self.stateMatrix
        print("\nSubtracting measurement from state:")
        print(y)
        print()
        gain = self.calcKalmanGain()
        print("\nKalman Gain:")
        print(gain)
        print()
        print("\nGain * measurment from state:")
        print(gain * y)
        print()
        print("\nState Pre summation:")
        print(self.stateMatrix)
        print()
        print("\n")
        print()
        print()
        self.stateMatrix = self.stateMatrix + (gain * y)
        print("\nState post summation:")
        print(self.stateMatrix)
        print()
        self.updateError(gain)

    def updateCovariance(self):         #accumulates the error that comes from motion/adding gaussians together
        self.stateErrors = self.transitionMatrix * self.stateErrors * self.transitionMatrix.transpose()
        print("\nNew covariance:")
        print(self.stateErrors)
        print()
                    
    def updateState(self, timeStep): #uses motion model to estimate the new state after a given timestep
        self.stateMatrix = self.transitionMatrix * self.stateMatrix
        if(self.externalMotion != None):
            self.stateMatrix += self.externalTransition * self.externalMotion
        self.updateCovariance()
        

if __name__ == "__main__":
    timeStep = 1
##    stateMatrix = Matrix([[0],[0]])
##    stateErrors = Matrix([[1000, 0], [0, 1000]])
##    transitionMatrix = Matrix([[1, timeStep],[0, 1]])
##    measureMatrix = Matrix([[1, 0]])
##    measureErrors = Matrix([[1]])
##    externalTransition = Matrix([[.5 * math.pow(timeStep, 2)], [timeStep]])
##    externalMotion = Matrix([[0]])
    stateMatrix = Matrix([[1],[0]])
    stateErrors = Matrix([[100, 0], [0, 100]])
    transitionMatrix = Matrix([[1, timeStep],[0, 1]])
    measureMatrix = Matrix([[1, 0]])
    measureErrors = Matrix([[200]])
    externalTransition = Matrix([[.5 * math.pow(timeStep, 2)], [timeStep]])
    externalMotion = Matrix([[0]])
    
    fil = KalmanFilter(stateMatrix, stateErrors, transitionMatrix, measureMatrix, measureErrors, externalTransition, externalMotion)
    print(stateMatrix)
    print(stateErrors)
    print('-' * 20)
    print('\n')
    sensorNoiseMag = 1
    actualStates = [x for x in range(1, 20)]
    #actualStates = [1 + .5 * math.pow(x, 2) for x in range(1, 20)]
    states = []
    measurements = [x + random.randint(-1,1) * random.random() * sensorNoiseMag for x in range(1, 20)]
    #measurements = [1 + .5 * math.pow(x, 2) + random.randint(-sensorNoiseMag, sensorNoiseMag) * random.random() for x in range(1, 20)]
    for measurement in measurements:
        print("Measurement:")
        print(measurement)
        print()
        fil.updateEstimate([measurement])
        states.append(fil.stateMatrix[0][0])
        print('Post measurment: ')
        print(fil.stateMatrix)
        print()
        fil.updateState(timeStep)
        print('Post new state Estimate: ')
        print(fil.stateMatrix)
        print('\n')
        print('-' * 20)
        print('\n')
        #states.append(fil.stateMatrix[0][0])
    print(fil.stateErrors)
    print()
    print(actualStates[-1])
    time = [x for x in range(0, 19)]
    plt.plot(time, measurements, label= "measured states")
    plt.plot(time, actualStates, label= "actual state")
    plt.plot(time, states, label= "estimated states")
    plt.xlabel("Time(s)")
    plt.ylabel("State(m)")
    plt.legend(loc = "upper left")
    plt.show()
