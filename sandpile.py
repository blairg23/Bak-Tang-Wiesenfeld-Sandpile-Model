from pylab import *
import matplotlib.pyplot as plt
import os

numberBreaks = 0

#Save a file with a given filename and directory
#Creates directory if it doesn't exist.
def saveFile(fileName, directory):
    if not os.path.exists(directory):
        os.makedirs(directory)
    savefig(fileName)

        
#Sandpile increase function:
def increase(m, x, y, critLevel,N):
    global numberBreaks
    isBoundary = False
    #If we reached a boundary:
    if x < 0 or x >= N:
        isBoundary = True
        pass
    if y < 0 or y >= N:
        isBoundary = True
        pass
    #If we're not on a boundary,
    if isBoundary == False:
        #Increment the current element
        m[x][y] += 1
        numberBreaks += 1
#        print 'number of breaks: ' + str(numberBreaks)
#        imshow(m)
#        draw()
        #If we're past the critical number:
        if m[x][y] >= critLevel:
            m[x][y] -= 4
            increase(m,x+1,y,critLevel,N)
            increase(m,x-1,y,critLevel,N)
            increase(m,x,y+1,critLevel,N)
            increase(m,x,y-1,critLevel,N)


#Dribble sand in random x,y coordinate
def dribbleRandom(m,critLevel,N):
    x = randint(N-1)
    y = randint(N-1)
    increase(m, x, y, critLevel,N)


#Dribble sand in specified x,y coordinate
def dribbleOnPoint(m,x,y,critLevel,N):
    increase(m, x, y, critLevel,N)


#Initialize constants:
N = 21 #Size of matrix
p = .001 #Amount of change each grain undergoes
critLevel = 4 #Critical Level
iterations = 50#50, 250, 500, 1000, 1500, 2000, 2500, 5000, 10000, 25000, 50000]


#Initialize modes:
mode = 'SpecificMiddle' #Modes: Random, SpecificCorners, SpecificMiddle
initMode = 'OverCrit' #Modes: RandomInt, RandomFloat, Static, NearCrit, OverCrit
animated = True #Set to true if you want to see animation
save = False#Set to true to save the file

#Matrix of grains of sand:
if initMode == 'RandomFloat':
    z = random(size=(N,N)) #Initialized with random floating point value
elif initMode == 'RandomInt':
    z = randint(critLevel, size=(N,N)) #With random integer values (not at the crit level)
elif initMode == 'Static':
    z = zeros((N,N))     #All zeros
elif initMode == 'NearCrit':
    z = ones((N,N)) * 3. #Near critical point
elif initMode == 'OverCrit':
    z = ones((N,N)) * 7. #Over critical point

#For plotting the number of breaks (avalanches):
breakPlot = []
freqPlot = {}

#Plot variables:
fontFamily = 'serif'
fontSize = '25'

#Plot matrix here:
ion()
clf()

F = gcf()
Size = F.get_size_inches()
F.set_size_inches(Size[0]*2, Size[1]*2, forward=True)#Set forward to True to resize window along with plot in figure.

plt.imshow(z, vmin=0, vmax=critLevel, alpha=1)

#Colorbar stuff:
cb = colorbar(orientation='horizontal')
cb.set_label('Z-Value Represents Slope (' + str(critLevel) + ' is the critical value)', 
             size=int(fontSize)/1.5, family=fontFamily)
#Text box:
fig_text = figtext(0.75,0.75, 'Iterations: ' + str(iterations) + '\n'
                   'Init Mode: ' + str(initMode) + '\n'
                   'Dribble Mode: ' + str(mode), color='white', 
                    backgroundcolor='black', size=int(fontSize)/1.5,
                    family=fontFamily)
title(r'Per Bak Sandpile Model', size=fontSize, family=fontFamily)
grid(color="Grey") #Enables grid

#Set axes:
xlabel('x-coordinate', size = int(fontSize)/1.5, family=fontFamily)
ylabel('y-coordinate', size= int(fontSize)/1.5, family=fontFamily)
xlim(0,N-1)
ylim(0,N-1)

if mode == 'Random':
    freqPlot = {}
    for i in range(iterations):
        numberBreaks = 0 #Reset the number of breaks
        dribbleRandom(z, critLevel,N)
        fig_text.set_text('Iterations: ' + str(i+1) + '\n'
                          'Init Mode: ' + str(initMode) + '\n'
                          'Dribble Mode: ' + str(mode) + '\n'
                          'Breaks: ' + str(numberBreaks) )

        #Record the number of breaks:
        breakPlot.append(numberBreaks)
        if numberBreaks in freqPlot:
            freqPlot[numberBreaks] += 1
        else:
            freqPlot[numberBreaks] = 1

        z = array(z)
        if animated:
            plt.imshow(z)
            plt.draw()
    plt.imshow(z)

    if save:
        fileName = 'images/' + str(initMode) + '_' + str(mode) + '/' + str(initMode) + '_' + str(mode) + '_' + str(i+1) + '.png'
        directory = 'images/' + str(initMode) + '_' + str(mode) + '/'
        saveFile(fileName, directory)        
        draw()        



if mode == 'SpecificCorners':
    freqPlot = {}
    for i in range(iterations):
        numberBreaks = 0 #Reset the number of breaks
        x = 0
        y = 0
        dribbleOnPoint(z,x,y,critLevel,N)
        fig_text.set_text('Iterations: ' + str(i+1) + '\n'
                          'Init Mode: ' + str(initMode) + '\n'
                          'Dribble Mode: ' + str(mode) + '\n'
                          'Breaks: ' + str(numberBreaks) )
        #Record the number of breaks:
        breakPlot.append(numberBreaks)
        if numberBreaks in freqPlot:
            freqPlot[numberBreaks] += 1
        else:
            freqPlot[numberBreaks] = 1


        x = len(z) -1
        y = 0
        dribbleOnPoint(z,x,y,critLevel,N)
        
        x = 0
        y = len(z)-1
        dribbleOnPoint(z,x,y,critLevel,N)

        x = len(z)-1
        y = len(z)-1
        dribbleOnPoint(z,x,y,critLevel,N)
        
        if animated:
            plt.imshow(z)
            plt.draw()

    plt.imshow(z)

    if save:
        fileName = 'images/' + str(initMode) + '_' + str(mode) + '/' + str(initMode) + '_' + str(mode) + '_' + str(i+1) + '.png'
        directory = 'images/' + str(initMode) + '_' + str(mode) + '/'
        saveFile(fileName, directory) 
        show()

    

if mode == 'SpecificMiddle':
    freqPlot = {}
    for i in range(iterations):
        numberBreaks = 0 #Reset the number of breaks
        x = floor(len(z)/2)
        y = floor(len(z)/2)
        dribbleOnPoint(z,x,y,critLevel,N)
        fig_text.set_text('Iterations: ' + str(i+1) + '\n'
                          'Init Mode: ' + str(initMode) + '\n'
                          'Dribble Mode: ' + str(mode) + '\n'
                          'Breaks: ' + str(numberBreaks) + '\n'
                          'Specified Point: [' + str(int(x)) + ',' + str(int(y)) + ']')
        #Record the number of breaks:
        breakPlot.append(numberBreaks)
        if numberBreaks in freqPlot:
            freqPlot[numberBreaks] += 1
        else:
            freqPlot[numberBreaks] = 1


        if animated:
            plt.imshow(z)
            plt.draw()
    
    plt.imshow(z)

    if save:
        fileName = 'images/' + str(initMode) + '_' + str(mode) + '/' + str(initMode) + '_' + str(mode) + '_' + str(i+1) + '.png'
        directory = 'images/' + str(initMode) + '_' + str(mode) + '/'
        saveFile(fileName, directory)
        show()

ioff()

breakPlot = array(breakPlot)

#For plotting the size vs. frequency of occurence:
size = []
size = sorted(freqPlot.keys())
freq = []
for s in size:
    freq.append(freqPlot[s])

print 'Size of Avalanches:' + str(size)
print 'Frequency of Avalanches:' + str(freq)
"""
#THIS ONE WORKS:
clf()
figure(0)
loglog(size, freq, 'go-')
title('Size of Avalanche vs. Frequency of Occurrence ($i = %d$ iterations)' %iterations, family=fontFamily, size=fontSize)
xlabel('Size (grains)', family=fontFamily, size=int(fontSize)/1.5)
ylabel('Frequency', family=fontFamily, size=int(fontSize)/1.5)
#Text box:
fig_text = figtext(0.75,0.75, 'Iterations: ' + str(iterations) + '\n'
                   'Init Mode: ' + str(initMode) + '\n'
                   'Dribble Mode: ' + str(mode), color='white', 
                    backgroundcolor='black', size=int(fontSize)/1.5,
                    family=fontFamily)
show()
"""


"""
clf()
figure(0)
plot(breakPlot, 'go-')
title('Avalanches per Iteration ($i = %d$ iterations)' % iterations, family=fontFamily, size=fontSize)
xlabel('Iterations', family=fontFamily, size=int(fontSize)/1.5)
ylabel('Number of Avalanches', family=fontFamily, size=int(fontSize)/1.5)
#Text box:
fig_text = figtext(0.75,0.85, 'Iterations: ' + str(iterations) + '\n'
                   'Init Mode: ' + str(initMode) + '\n'
                   'Dribble Mode: ' + str(mode), color='white', 
                    backgroundcolor='black', size=int(fontSize)/1.5,
                    family=fontFamily)
fileName = 'images/' + str(initMode) + '_' + str(mode) + '/plot' + str(initMode) + '_' + str(mode) + '_' + str(i+1) + '.png'
#savefig(fileName)
show()


clf()
figure(1)
loglog(breakPlot, 'ro-')
title('Log-Base Plot of Avalanches per Iteration ($i = %d$ iterations)' % iterations, family=fontFamily, size=fontSize)
xlabel('Iterations', family=fontFamily, size=int(fontSize)/1.5)
ylabel('Number of Avalanches', family=fontFamily, size=int(fontSize)/1.5)
#Text box:
fig_text = figtext(0.15,0.8, 'Iterations: ' + str(iterations) + '\n'
                   'Init Mode: ' + str(initMode) + '\n'
                   'Dribble Mode: ' + str(mode), color='white', 
                    backgroundcolor='black', size=int(fontSize)/1.5,
                    family=fontFamily)
fileName = 'images/' + str(initMode) + '_' + str(mode) + '/logPlot' + str(initMode) + '_' + str(mode) + '_' + str(i+1) + '.png'
#savefig(fileName)
show()
"""


