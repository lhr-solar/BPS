from __future__ import division
import math
from math import sqrt
from operator import *
class Matrix():

    def __init__(self, matrix):
        self.matrix = matrix                                                #2D array, each element is a row
        self.rows = len(matrix)                                             #so the number of elements is the number of rows
        self.columns = len(matrix[0])                                       #and the length of an element(or row) is the number of columns

        for x in matrix:                                                    #checking if they put in a valid matrix 
            if len(x) != self.columns:
                raise ValueError('All rows must be of the same length')     

    def __add__(self, b):                                                   #edits what happens when two matricies are added
        if isinstance(b, Matrix):
            if b.rows != self.rows or b.columns != self.columns:                #if the matrices are not the same size raise an error
                raise ValueError('Matricies must be of the same size')

            result = Matrix.zero(self.rows, self.columns)                       # creates a zero matrix of the same size
            
            for x in range(self.rows):                                          #sets each value in the matrix equal to the sum of its respective counterparts in the other two matrices
                for y in range(self.columns):
                    result.matrix[x][y] = self.matrix[x][y] + b.matrix[x][y]
        else:
            raise ValueError('Unsupported operands for + between type Matrix and type %s' % type(b))
        
        return result

    def __sub__(self, b):                                                   #edits what happens when two matricies are added
        if isinstance(b, Matrix):
            if b.rows != self.rows or b.columns != self.columns:                #if the matrices are not the same size raise an error
                raise ValueError('Matricies must be of the same size')

            result = Matrix.zero(self.rows, self.columns)                              # creates a zero matrix of the same size
            
            for x in range(self.rows):                                          #sets each value in the matrix equal to the sum of its respective counterparts in the other two matrices
                for y in range(self.columns):
                    result.matrix[x][y] = self.matrix[x][y] - b.matrix[x][y]
        else:
            raise ValueError('Unsupported operands for - between type Matrix and type %s' % type(b))
        
        return result

    def __mul__(self, b):                                                   #edits what happens when two matricies are multiplied
        if isinstance(b, Matrix):
            if self.columns != b.rows:
                raise ValueError('The columns of the first matrix must equal the number of the rows of the second')   #raise error if the matrices are not of the right side

            result = Matrix.zero(self.rows, b.columns)                                         #creates empty matrix of the rows of the first, and the columns of the second

            for x in range(self.rows):                                                  #goes through each row and each column of the empty matrix
                for y in range(b.columns):                               
                    for k in range(self.columns):                                       #adds the products of each respective pair from the first's row and the second's column
                        result.matrix[x][y] += self.matrix[x][k] * b.matrix[k][y]
        elif isinstance(b, int) or isinstance(b, float):
            result = Matrix.zero(self.rows, self.columns)
            for x in range(self.rows):
                for y in range(self.columns):
                    result.matrix[x][y] = self.matrix[x][y] * b
        else:
            raise ValueError('Unsupported operands for * between type Matrix and type %s' % type(b))
        
        return result

    def __truediv__(self, b):
        if isinstance(b, Matrix):               #if b is a mtrix just multiply by the inverse of that matrix
            result = self * b.inverse()

        elif isinstance(b, int) or isinstance(b, float):                #otherwise divide every component of the matrix by the constant
            result = Matrix.zero(self.rows, self.columns)
            for x in range(self.rows):
                for y in range(self.columns):
                    result.matrix[x][y] = self.matrix[x][y] / b
    
        else:
            raise ValueError('Unsupported operands for / between type Matrix and type %s' % type(b))

        return result

    def __floordiv__(self, b):
        return self / float(b)

    def __getitem__(self, key):
        try:
            return self.matrix[key]
        except Exception as e:
            raise Exception(e)
            
    def transpose(matrix):                                                  #static, returns the transpose of an input matrix
        result = Matrix.zero(matrix.columns, matrix.rows)                   #makes zero matrix of opposite dimensions
        for x in range(matrix.rows):                                        #goes through every row and column and sets the mirrored spot in the new matrix to that same value
            for y in range(matrix.columns):
                result.matrix[y][x] = matrix.matrix[x][y]
                
        return result
    
    def transpose(self):                                                    #non-static version, returns the transpose of the instinces matrix
        result = Matrix.zero(self.columns, self.rows)                       #look at the first version above for a description
        for x in range(self.rows):                                          
            for y in range(self.columns):
                result.matrix[y][x] = self.matrix[x][y]
                
        return result
    
    def zero(rows, columns):                                                #creates a matrix of zeros of the input dimensions
        if rows < 1 or columns < 1:
            raise ValueError('Invalid Dimensions, must be greater than zero')
        matrix = []
        for x in range(rows):                                               #will run through and create each row
            row = []                                                        #initializes an empty list for each row
            for y in range(columns):                                        #will go down the length of each row
                row.append(0)                                               #will fill it up with zeros
            matrix.append(row)                                              #will append that row

        return Matrix(matrix)      

    def identity(dimension):                                                #creates an identity matrix of the input dimensions(must be a square matrix)
        if dimension < 1:
            raise ValueError('Invalid Dimensions, must be greater than zero')
        result = Matrix.zero(dimension, dimension)     
        for x in range(dimension):                                          #just goes through each row and puts a one into the index of that row number
            result.matrix[x][x] = 1

        return result

    def Cholesky(self, ztol=1.0e-5):                                #this is and the Cholesky inverse are the only two pieces I did NOT program. Credit to Udacity
        # Computes the upper triangular Cholesky factorization of
        # a positive definite matrix.
        res = Matrix([[]])
        res = Matrix.zero(self.rows, self.columns)

        for i in range(self.rows):
            S = sum([(res.matrix[k][i])**2 for k in range(i)])
            d = self.matrix[i][i] - S
            if abs(d) < ztol:
                res.matrix[i][i] = 0.0
            else:
                if d < 0.0:
                    raise ValueError("Matrix not positive-definite")
                res.matrix[i][i] = sqrt(d)
            for j in range(i+1, self.rows):
                S = sum([res.matrix[k][i] * res.matrix[k][j] for k in range(self.rows)])
                if abs(S) < ztol:
                    S = 0.0
                res.matrix[i][j] = (self.matrix[i][j] - S)/res.matrix[i][i]
        return res
    
    def CholeskyInverse(self):
        # Computes inverse of matrix given its Cholesky upper Triangular
        # decomposition of matrix.
        res = Matrix([[]])
        res = Matrix.zero(self.rows, self.columns)
        
        # Backward step for inverse.
        for j in reversed(range(self.rows)):
            tjj = self.matrix[j][j]
            S = sum([self.matrix[j][k]*res.matrix[j][k] for k in range(j+1, self.rows)])
            res.matrix[j][j] = 1.0/tjj**2 - S/tjj
            for i in reversed(range(j)):
                res.matrix[j][i] = res.matrix[i][j] = -sum([self.matrix[i][k]*res.matrix[k][j] for k in range(i+1, self.rows)])/self.matrix[i][i]
        return res
    

    def inverse(self):
        aux = self.Cholesky()
        result = aux.CholeskyInverse()
        return result

    def apply(self, opcode, operand = None):            #applys a certain operation(opcode) to the entire matrix, uses names from the operator library for this, the operand part is an optional number for stuff like add
        result = Matrix.zero(self.rows, self.columns)
        for x in range(self.rows):
            for y in range(self.columns):                                   #going through every row and column
                if operand:
                    result.matrix[x][y] = opcode(self.matrix[x][y], operand) #the opcode is applied like a function
                else:
                    result.matrix[x][y] = opcode(self.matrix[x][y])
                    
        return result

    def applyRow(self, rowNum, opcode, operand = None):  #applys a certain operation to only one row, uses names from the operator library for this
        result = self.makeCopy()
        for y in range(self.columns):
            if isinstance(operand, list):                                           #can also specify a different operand for each specific spot in the list
                result.matrix[rowNum][y] = opcode(self.matrix[rowNum][y], operand[y])
            elif operand:                                                            #otherwise we just apply the opcode and operand to every spot
                result.matrix[rowNum][y] = opcode(self.matrix[rowNum][y], operand)
            else: 
                result.matrix[rowNum][y] = opcode(self.matrix[rowNum][y])           #or just the opcode
                        
        return result

    def applyRows(self, rowNums, opcode, operand = None):       #apply an operation to multiple rows, you have the option of maing operands a list as well
        result = self.makeCopy()
        for index, rowNum in enumerate(rowNums):
            for y in range(self.columns):
                if isinstance(operand, list):
                    result.matrix[rowNum][y] = opcode(self.matrix[rowNum][y], operand[index])
                elif operand:
                    result.matrix[rowNum][y] = opcode(self.matrix[rowNum][y], operand)
                else:
                    result.matrix[rowNum][y] = opcode(self.matrix[rowNum][y])
            
        return result

    def applyRowOps(self, rowNums, opcodes, operands = None): #apply a list of operations each to its repsective row in row nums, oeprands can also be a list or 2D list
        result = self.makeCopy()
        for index, rowNum in enumerate(rowNums):
            for y in range(self.columns):
                if isinstance(operands, list):
                    if isinstance(operands[index], list):
                        result.matrix[rowNum][y] = opcodes[index](self.matrix[rowNum][y], operands[index][y])
                    else:
                        result.matrix[rowNum][y] = opcodes[index](self.matrix[rowNum][y], operands[index])
                elif operands:
                    result.matrix[rowNum][y] = opcodes[index](self.matrix[rowNum][y], operands)
                else:
                    result.matrix[rowNum][y] = opcodes[index](self.matrix[rowNum][y])
                    
        return result

    ###ALL THE APPLYCOLUMNSXXX FUNCTIONS DO THE SAME AS APPLYROWSXXX ABOVE BUT TO COLUMNS, and there is a minor change on how to cycle through columns than rows###
    def applyColumn(self, columnNum, opcode, operand = None):
        result = self.makeCopy()

        for x in range(self.rows):
            if isinstance(operand, list):
                result.matrix[x][columnNum] = opcode(self.matrix[x][columnNum], operand[x]) #here we go down rows instead of columns, columnNum is held constant
            elif operand:
                result.matrix[x][columnNum] = opcode(self.matrix[x][columnNum], operand)
            else:
                result.matrix[x][columnNum] = opcode(self.matrix[x][columnNum])
                
        return result

    ###ALL THE APPLYCOLUMNSXXX FUNCTIONS DO THE SAME AS APPLYROWSXXX BUT TO COLUMNS, read them above for more info###
    def applyColumns(self, columnNums, opcode, operand = None):
        result = self.makeCopy()

        for index, columnNum in enumerate(columnNums):
            for x in range(self.rows):
                if isinstance(operand, list):
                    result.matrix[x][columnNum] = opcode(self.matrix[x][columnNum], operand[index])
                elif operand:
                    result.matrix[x][columnNum] = opcode(self.matrix[x][columnNum], operand)
                else:
                    result.matrix[x][columnNum] = opcode(self.matrix[x][columnNum])

        return result

    ###ALL THE APPLYCOLUMNSXXX FUNCTIONS DO THE SAME AS APPLYROWSXXX BUT TO COLUMNS, read them above for more info###
    def applyColumnOps(self, columnNums, opcodes, operand = None):
        result = self.makeCopy()

        for index, columnNum in enumerate(columnNums):
            for x in range(self.rows):
                if isinstance(operand, list):
                    result.matrix[x][columnNum] = opcodes[index](self.matrix[x][columnNum], operand[index])
                elif operand:
                    result.matrix[x][columnNum] = opcodes[index](self.matrix[x][columnNum], operand)
                else:
                    result.matrix[x][columnNum] = opcodes[index](self.matrix[x][columnNum])

        return result
    
    def swapRows(self, row1, row2): #simply swaps two rows
        result = self.makeCopy()
        temp = result.matrix[row1]
        result.matrix[row1] = result.matrix[row2]
        result.matrix[row2] = temp
        
        return result
        
    def swapColumns(self, column1, column2): #swaps two columns
        result = self.makeCopy()
        if column1 - column2 == 0: #if the same column num is put in twice we just ignore the swap
            return result
        
        else:
            for x in range(self.rows):  #we have to go through each row and swap the repsective numbers from each column
                temp = result.matrix[x][column1]
                result.matrix[x][column1] = result.matrix[x][column2]
                result.matrix[x][column2] = temp
                
        return result


    def rowOp(self, row1, row2, opcode, rowSave = 0):       #applys an operation element by element between two rowsits always row2 acting on row1, Ex: for sub its row1 - row2
        result = self.makeCopy()                            #Ex: for sub its row1 - row2

        if rowSave > 1: #if 0 we save over row1, if rowSave is 1 we save over row2
            raise ValueError('rowSave flag  must only be set to zero and one')
        for y in range(self.columns):
            result.matrix[row1 * (1 - rowSave) + row2 * rowSave][y] = opcode(result.matrix[row1][y], result.matrix[row2][y])

        return result
    
    def makeCopy(self): #returns a copy of the current matrix for us to manipulate without changing the current matrix
        result = Matrix.zero(self.rows, self.columns)
        for x in range(self.rows):
            for y in range(self.columns):
                result.matrix[x][y] = self.matrix[x][y]
                
        return result

    def subMatrix(self, rowRange, columnRange): #creates and returns subMatrix of the specified rows and columns
        newMatrix = Matrix.zero(rowRange[1] - rowRange[0] + 1, columnRange[1] - columnRange[0] + 1)
        i = 0
        for x in range(rowRange[0], rowRange[1] + 1):
            j = 0
            for y in range(columnRange[0], columnRange[1] + 1):
                newMatrix[i][j] = self.matrix[x][y]
                j += 1
            i += 1
            
        return newMatrix

    def where(self, opcode, operand = None): #applys boolean operation to matrix and returns ones where it is satisfied
        newMatrix = self.makeCopy()
        newMatrix = newMatrix.apply(opcode, operand)
        indices = []
        for i in range(newMatrix.rows):
            for j in range(newMatrix.columns):
                if newMatrix[i][j] == 1:
                    indices.append([i, j])
        return newMatrix, indices

    def flatten(self): #flattens the matrix, aka puts all elements into a one dimensional list
        flatList = []
        for row in self.matrix:
            for element in row:
                flatList.append(element)
        return flatList
    
    def determinant(self, matrix = None, row = 0):       #solves for the determinant of a matrix, uses recursion, row is the row to multip
        if matrix == None:
            matrix = self.makeCopy()
            
        if matrix.rows != matrix.columns:
            raise ValueError('Matrix must be a square matrix to calculate the determinant')
        elif matrix.rows == 2:  #if a two by two then use the formula
            return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]
        else:   #else here is where the recursion comes in
            total = 0
            for y in range(matrix.columns):
                if matrix[row][y] == 0:
                    continue
                newRows = []
                for x in range(matrix.rows): #creates the smaller sub matrix that we must calculate the sub determinant of
                    if row == x:
                        continue
                    newRow = []
                    for z in range(matrix.columns):
                        if z == y:
                            continue
                        else:
                            newRow.append(matrix[x][z])
                    newRows.append(newRow)
                newMatrix = Matrix(newRows)
                total += matrix[row][y] * self.determinant(newMatrix, row) * int(math.pow(-1, y)) #adds the derivative of the sub matrix times the constant to the total
            return total


    def reducedRowForm(self, solutionsMatrix): #turns a matrix into reduced row echelon form
        newMatrix = self.makeCopy()
        newSolutions = solutionsMatrix.makeCopy()

        for y in range(newMatrix.columns):          #if the colum is all zeroes we can skip it so we check for that
            allZeroes = True
            if newMatrix[y][y] == 0:
                for x in range(1 + y, newMatrix.rows):
                    if newMatrix[x][y] != 0:
                        allZeroes = False
                        newMatrix = newMatrix.swapRows(y, x) #we swap the current row with the first row that has a non-zero term in this column
                        newSolutions = newSolutions.swapRows(y, x)
                        break;
            else:
                allZeroes = False
                
            if allZeroes:
                continue
            
            for x in range(1 + y, newMatrix.rows):  #here we clear columns below each pivot variable
                if newMatrix[x][y] != 0:
                    alpha = newMatrix[x][y] / float(newMatrix[y][y]) #alpha is the ratio between the pivot's constant and the other variables constant
                    newMatrix = newMatrix.applyRow(y, mul, alpha)  #multiply pivots row by alpha to make its constant equal to the other variables
                    newSolutions = newSolutions.applyRow(y, mul, alpha) #apply to the solutions as well
                    newMatrix = newMatrix.rowOp(x,y,sub,0)  #subtract the two rows and overwrite the non pivot variable row
                    newSolutions = newSolutions.rowOp(x, y, sub, 0) #apply to solutions as well
                    newMatrix = newMatrix.applyRow(y, mul, 1 / alpha)
                    newSolutions = newSolutions.applyRow(y, mul, 1 / alpha)
            
        return newMatrix, newSolutions

    def equationSolver(self, solutionsMatrix):      #solves lienar systems of equations
        if self.rows != solutionsMatrix.rows:
            raise ValueError("Must be as many solutions as there are equations(Matrix must have equal rows as the solutions matrix)")
        newMatrix, newSolutions = self.reducedRowForm(solutionsMatrix) #gets it in reduced row echeclon form

        #check for no solutions
        for x in range(newMatrix.rows): #if there is a row of all zeroes equal to a constant, no solutions, otherwise infintie solutions
            allZeroes = True
            for y in range(newMatrix.columns):
                if not Matrix.isZero(newMatrix[x][y]):
                    allZeroes = False
                    break;
            if allZeroes and not Matrix.isZero(newSolutions[x][0]):
                return newMatrix, 'NO SOLUTION'
            elif allZeroes and Matrix.isZero(newSolutions[x][0]):                                              #infinite solutions, add in paramtertization
                return newMatrix, 'INFINITE SOLUTIONS'
                #parameteritize solution
            
        #solve solutions
        for x in range(newMatrix.rows - 1, -1, -1): #going backwards through the rows
            y = 0
            while newMatrix[x][y] == 0: #finding first non zero term
                y += 1
            if y != newMatrix.columns - 1:  #going through remaining terms and subtracting them from the solution
                k = y + 1
                while k < newMatrix.columns:    
                    newSolutions[x][0] -= newMatrix[x][k]
                    newMatrix[x][k] = 0
                    k += 1
            
            num = newMatrix[x][y]   
            newMatrix = newMatrix.applyRow(x, truediv, num) #dividing through by the varibales constant to set it to one and solve for it
            newSolutions = newSolutions.applyRow(x, truediv, num)
            for z in range(0, x): #going back up through the rows and substituing back in the solution
                newMatrix[z][y] = newSolutions[x][0] * newMatrix[z][y]
                
        return newMatrix, newSolutions

    def isZero(x):          #used if we are ever working with small floats that we will just call zero
        return abs(x) < (0 + .001) 
    
    def __eq__(self, b):            #for equality testing between matrices
        if not isinstance(b, Matrix):
            return False
        else:
            return self.matrix == b.matrix

    def __len__(self):
        return len(self.matrix)
    
    def __str__(self):                                                      #just prints out every row when you try to print the class
        matrixStr = ''
        for x in range(self.rows):
            matrixStr += '['
            for y in range(self.columns):
                if isinstance(self.matrix[x][y], list):
                    matrixStr += '['
                    for i, elem in enumerate(self.matrix[x][y]):
                        matrixStr += str(elem)
                        if i < len(self.matrix[x][y]) - 1:
                            matrixStr += ', '
                    matrixStr += ']'
                    if (y < self.columns - 1):
                        matrixStr += ', '
                elif isinstance(self.matrix[x][y], int) or isinstance(self.matrix[x][y], float):
                    num = self.matrix[x][y]
                    if num >= 0 and y > 0:
                        matrixStr += ' '
                    if round(num) - num != 0:
                        matrixStr += ("%.2f" % float(self.matrix[x][y]))
                    else:
                        matrixStr += str(int(num))
                    if (y < self.columns - 1):
                        matrixStr += ', '
                else:
                    if y > 0:
                        matrixStr += ' '
                    matrixStr += str(self.matrix[x][y])
                    if (y < self.columns - 1):
                        matrixStr += ', '
            matrixStr += ']'
            if(x < self.rows - 1):
                matrixStr += '\n'
        
        return matrixStr

    def __value__(self):    #returns the matrix so you dont need to call ###.matrix[][] every time you can just call ###[][]
        return self.matrix
    
if __name__ == "__main__":
    #matrix = Matrix([[2,3], [1,7]]) #just test code
    #matrix2 = Matrix([[200, 100],[100, 200]])
    #matrix3 = matrix2.transpose()
    #solutions = Matrix([[5], [10]])
    #newMatrix, solutions = matrix.equationSolver(solutions)
    #print(solutions)
    matrix4 = Matrix([[1,2,3], [1,2,3], [1,2,3]])
    matrix, indices = matrix4.where(ge, 2)
    print(matrix)
    print()
    print(indices)
