import time

class PID():

    def __init__(self, desiredState, pro = 0, integral = 0, deriv = 0):
        self.proConstant = pro
        self.intConstant = integral
        self.derConstant = deriv
        self.desiredState = desiredState
        self.integral = 0
        self.lastError = 0
        self.lastTime = int(round(time.time() * 1000))
        
    def calcError(self, state):
        if isinstance(self.desiredState, int):
            return self.desiredState - state
        elif isinstance(self.desiredState, list):
            return sum([x - y for x,y in zip(self.desiredState, state)])    

    def calcProportional(self, error):
        return self.proConstant * error

    def calcIntegral(self, error):
        self.integral += self.intConstant * error * self.lastTime
        return self.integral

    def calcDerivative(self, error):
        if self.changed:
            error = self.lastError
        return self.derConstant * (error - self.lastError) / self.lastTime

    def calcOutput(self, state):
        error = self.calcError(state)
        pro = self.calcProportional(error)
        integral = self.calcIntegral(error)
        deriv = self.calcDerivative(error)
        
        self.lastError = error
        self.lastTime = int(round(time.time() * 1000))
        if self.changed:
            self.changed = False
            
        return pro + integral + deriv
    
    def changeState(self, newState):
        self.desiredState = newState
        self.changed = True
        
if __name__ == "__main__":
    pass
