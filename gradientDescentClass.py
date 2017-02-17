import matrix
import math

class gradientDescent():

    def __init__(self, trainingData, weights, alpha, featureScaling = False, dp = None):
        self.trainingData = trainingData
        self.inputs = [x[0] for x in trainingData]
        self.outputs = [y[1] for y in trainingData]
        self.weights = weights
        self.alpha = alpha
        self.featureScaling = featureScaling
        if dp == None:
            self.dp = 6
        else:
            self.dp = dp

        if len(self.inputs[0]) != len(weights):
            raise ValueError('Weights do not match all the inputs!')

        if featureScaling:
            self.inputs, self.ratios = self.scaleFeatures()

        self.cleanUp()

    def cleanUp(self):
        for i in range(len(self.weights)):
            if isinstance(self.weights[i], list):
                for j in range(len(self.weights[i])):
                    self.weights[i][j] = round(self.weights[i][j], self.dp)
            else:
                self.weights[i] = round(self.weights[i], self.dp)
            
        for i in range(len(self.trainingData)):
            for j in range(len(self.trainingData[0][0])):
                self.trainingData[i][0][j] = round(self.trainingData[i][0][j], self.dp)
            self.trainingData[i][1] = round(self.trainingData[i][1], self.dp)

    def scaleFeatures(self, inputs = None):
        ratios = []
        if not inputs:
            inputs = self.inputs

        for i in range(1, len(inputs[0])):
            minVal = float(inputs[0][i])
            maxVal = float(inputs[0][i])
            for dataPoint in inputs:
                if dataPoint[i] < minVal:
                    minVal = float(dataPoint[i])
                elif dataPoint[i] > maxVal:
                    maxVal = float(dataPoint[i])
                    
            for dataPoint in inputs:
                dataPoint[i] = (dataPoint[i] - minVal) / (maxVal - minVal) 

            ratios.append([maxVal, minVal])
            
        return inputs, ratios

    def unscaleFeatures(self, inputs = None, ratios = None):
        if not inputs:
            inputs = self.inputs

        if not ratios:
            ratios = self.ratios

        for i in range(1, len(inputs[0])):
            for j in range(len(inputs)):
                inputs[j][i] = inputs[j][i] * (ratios[i - 1][0] - ratios[i - 1][1]) + ratios[i - 1][1]
            
        return inputs

    def calcCost(self, inputs, output, derivative = False, num = None):
        expected = 0
        for i, weight in enumerate(self.weights):
            expected += weight * inputs[i]
        
        if not derivative:
            return .5 * ((expected - output) ** 2)
        else:
            return (expected - output) * inputs[num]
        
    def calcAverageError(self, data = None):
        total = 0.0
        if not data:
            data = self.trainingData
            
        for dataPoint in data:
            total += self.calcCost(dataPoint[0], dataPoint[1])
        total = total / len(data)
        return total
        
    def adjustWeights(self):
        tempWeights = []
        
        for i in range(len(self.weights)):
            total = 0.0
            for dataPoint in self.trainingData:
                total += self.calcCost(dataPoint[0], dataPoint[1], self.featureScaling, i)     
                #print(total)
            total = total / len(self.trainingData)
            tempWeights.append(self.weights[i] - self.alpha * total)

        return tempWeights
    
    def fit(self, numSteps, normal = False):
        if not normal:
            for i in range(numSteps):
                self.weights = self.adjustWeights()
            self.cleanUp()
            return self.weights
        else:
            pass

    def predict(self, inputs, featureScale = None):
        if featureScale == None:
            featureScale = self.featureScaling
        if featureScale:
            for i in range(1, len(inputs)):
                inputs[i] = (inputs[i] - self.ratios[i - 1][1]) / float((self.ratios[i - 1][0] - self.ratios[i - 1][1]))
        output = 0.0
        for i, weight in enumerate(self.weights):
            output += weight * inputs[i]
        return output

    def normalEquation(self):
        inputMatrix = matrix.Matrix(self.inputs)
        outputMatrix = matrix.Matrix([[y] for y in self.outputs])
        inverse = (inputMatrix.transpose() * inputMatrix).inverse()
        weights = inverse * inputMatrix.transpose() * outputMatrix
        return weights
    
    def __str__(self):
        string = 'Weights: '
        for weight in self.weights:
            string += str(weight) + ' '
        string += '\nData: '
        string += str(self.trainingData)
        string += '\nAlpha: ' + str(self.alpha)
        string += '\nFeature Scaling: ' + str(self.featureScaling)
        return string


##class expRegression(gradientDescent):
##
##    def __init__(self, trainingData, weights, alpha, featureScaling = False, dp = None):
##        super(expRegression, self).__init__(self, trainingData, weights, alpha, featureScaling = False, dp = None)
##        self.expWeights = [x[0] for x in self.weights]
##        self.inputWeights = [x[1] for x in self.weights]
##
##    def calcCost(self, inputs, output, derivative = False, num = None, num2 = None):
##        expected = 0
##        for i in range(len(inputs))
##            expected += self.inputWeights[i] * math.exp(self.expWeights[i] * inputs[i])
##
##        if not derivative:
##            return .5 * ((expected - output) ** 2)
##        else:
##            if not num2:
##                return (expected - output) * math.exp(self.expWeights[num] * inputs[num])
##            else:
##                return (expected - output) * self.expWeights[num] * self.inputWeights[num] * math.exp(self.expWeights[num] * inputs[num])
##            
##    def adjustWeights(self):
##        tempWeights = []
##        
##        for i in range(len(self.weights)):
##            total = 0.0
##            for dataPoint in self.trainingData:
##                total += self.calcCost(dataPoint[0], dataPoint[1], True, i)     
##                #print(total)
##            total = total / len(self.trainingData)
##            tempWeights.append(self.weights[i] - self.alpha * total)
##
##        return tempWeights        

        
class powerRegression(gradientDescent):

    def __init__(self, trainingData, weights, alpha, featureScaling = False, dp = None):
        super(powerRegression, self).__init__(trainingData, weights, alpha, featureScaling, dp)

    def calcCost(self, inputs, output, derivative = False, num = None):
        expected = 0
        for i in range(len(inputs)):
            if isinstance(self.weights[i], list):
                expected += self.weights[i][0] * (inputs[i] * self.weights[i][1]) ** 2
            else:
                expected += self.weights[i] * inputs[i]
                
        if derivative:
            if isinstance(self.weights[num], list):
                return [(expected - output) * (inputs[num] ** self.weights[num][1]), (expected - output) * (self.weights[num][0] * self.weights[num][1] * (inputs[num] ** (self.weights[num][1] - 1))), ]
            else:
                return (expected - output) * inputs[num]
        else:
            return .5 * (expected - output) ** 2
                
    def adjustWeights(self):
        tempWeights = []
            
        for i in range(len(self.weights)):
            total = 0.0
            totals = [0.0, 0.0]
            for dataPoint in self.trainingData:
                if isinstance(self.weights[i], list):
                    tempTotals = self.calcCost(dataPoint[0], dataPoint[1], True, i)
                    totals[0] = totals[0] + tempTotals[0]
                    totals[1] = totals[1] + tempTotals[1]
                else:
                    total += self.calcCost(dataPoint[0], dataPoint[1], True, i)

            if isinstance(self.weights[i], list):
                totals[0] = totals[0] / len(self.trainingData)
                totals[1] = totals[1] / len(self.trainingData)
                newWeights = [self.weights[i][0] - self.alpha * totals[0], self.weights[i][1] - self.alpha * totals[1]]
                tempWeights.append(newWeights)
            else:
                total = total / len(self.trainingData)
                tempWeights.append(self.weights[i] - self.alpha * total)

        return tempWeights

    def predict(self, inputs, featureScale = None):
        if featureScale == None:
            featureScale = self.featureScaling
        if featureScale:
            for i in range(1, len(inputs)):
                inputs[i] = (inputs[i] - self.ratios[i - 1][1]) / float((self.ratios[i - 1][0] - self.ratios[i - 1][1]))
        output = 0.0
        for i, weight in enumerate(self.weights):
            if isinstance(weight, list):
                output += weight[0] * (inputs[i] ** weight[1])
            else:
                output += weight * inputs[i]
        return output

##class logRegression(gradientDescent):
##
##    def __init__(self, trainingData, weights, alpha, featureScaling = False, dp = None, base = 10):
##        super(expRegression, self).__init__(self, trainingData, weights, alpha, featureScaling = False, dp = None)
##        self.base = base
##        
##    def calcCost(self, inputs, output, derivative = False, num = None):
##        expected = 0
##        for i in range(len(inputs))
##            expected += weight[i] * math.log(inputs[i], self.base)
##
##        if not derivative:
##            return .5 * ((expected - output) ** 2)
##        else:
##            return (expected - output) *  math.log(inputs[num])


if __name__ == "__main__":
##    trainingData = [[[1,4], 110],[[1,5], 135],[[1,6], 160],[[1,7], 185],[[1,8], 210]]
##    weights = [-5,-8]
##
##    grad = gradientDescent(trainingData, weights, .1, True, 3)
##    print(grad.fit(500))
##    print()
##    for i in range(len(trainingData)):
##        print(grad.inputs[i])
##        print(grad.outputs[i])
##        print(grad.predict(trainingData[i][0], False))
##        print()
##    print('Final Test:')
##    print(9 * 25 + 10)
##    print(grad.predict([1,9]))
##    print(grad.normalEquation())
    trainingDataP = [[[1, 2], 9],[[1, 3], 19],[[1, 4], 33],[[1, 5], 51],[[1, 6], 73]]
    weights2 = [1, [3, 2]]
    gradP = powerRegression(trainingDataP, weights2, .001, False, 3)
    print(gradP.predict([1,3]))
    print(gradP.calcCost([1, 2], 9))
    print(gradP.fit(20000))
    print(gradP.predict([1,3]))
