
Jack and Jill are playing a game they call Polygon Game. In this game they start with a convex polygon and then take turns dividing the polygon into smaller parts. They divide the polygon by drawing a straight line from one point on the border of the original polygon to another point on the border of the original polygon. The goal of the game is then to guess which of the resulting polygons has the largest area.
Input

#Input

The first line of the input consists of two integers
N, M: The number of vertices in the original polygon, and the number of lines drawn by Jack and Jill.
Then follows N lines, each consisting of two integers X and Y, representing the coordinates of the of the vertex in original polygon given in a counter-clockwise order.
Then follows M lines, each consisting four real numbers 

#Output

The output should be a single line consisting of the area of the largest polygon created. Your answer must have an absolute or relative error of at most


#Limits

3 < N < 25

2 < M < 50

M will be an even number.

X,Y < 1000

The polygon’s vertices are distinct.

All lines will start and end on the border of the original polygon with an absolute error of at most and all lines will divide the original polygon into two parts.

Real numbers in the input will have at most digits after the decimal point.