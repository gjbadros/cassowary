#!/uns/bin/python

from cassowary import *
from Tkinter import *

# TODO:
# leaving a green circle makes it blue

DOTSIZE = 5
MAXX = MAXY = 450

class App(Frame):
	def __init__(self, parent=None):
		# Widget initialization
		Frame.__init__(self)
		self.pack()
		self._canvas = Canvas(width=MAXX, height=MAXY)
		self._canvas.pack()

		# Local vars
		self._editX = None
		self._editY = None
		self._oldloc = {}		# Mapping from point to (x,y) location
		self._point_to_obj = {}	# Mapping from point to canvas obj

		# Create constraints, and refresh
		self.create_constraints()
		self.draw_first()

	def create_constraints(self):
		coords = [(50,50),(50,250),(250,250),(250,50)]
		self._points = map(
			lambda (x, y): ClPoint(x, y),
			coords)
		self._midpoints = map(lambda x: ClPoint(0, 0), coords)

		s = self._solver = ClSimplexSolver()
		weight = 0.5
		for p in self._points:
			s.addPointStay(p, weight)
			weight = weight * 2

		# Midpoints
		for i in range(4):
			a = i
			b = (i+1) % 4
			# X
			expr = ClLinearExpression(self._points[b].X())
			expr = expr.plus(ClLinearExpression(self._points[a].X()))
			expr = expr.divide(2)
			s.addConstraint(ClLinearEquation(self._midpoints[a].X(),
				expr, clsRequired()))
			# Y
			expr = ClLinearExpression(self._points[b].Y())
			expr = expr.plus(ClLinearExpression(self._points[a].Y()))
			expr = expr.divide(2)
			s.addConstraint(ClLinearEquation(self._midpoints[a].Y(),
				expr, clsRequired()))

		# Keep from turning inside out
		for (p1, p2) in [(0, 2), (0, 3), (1, 2), (1, 3)]:
			e = ClLinearExpression(self._points[p1].X())
			e = e.plus(ClLinearExpressionNum(DOTSIZE))
			c = ClLinearInequality(e, cnLEQ, ClLinearExpression(
				self._points[p2].X()),
				clsRequired())
			s.addConstraint(c)
		for (p1, p2) in [(0, 1), (0, 2), (3, 1), (3, 2)]:
			e = ClLinearExpression(self._points[p1].Y())
			e = e.plus(ClLinearExpressionNum(DOTSIZE))
			c = ClLinearInequality(e, cnLEQ, ClLinearExpression(
				self._points[p2].Y()),
				clsRequired())
			s.addConstraint(c)

		# Boundaries
		for p in self._points:
			x = p.X()
			y = p.Y()
			s.addConstraint(ClLinearInequality(ClLinearExpression(x), cnGEQ, ClLinearExpressionNum(0),clsRequired()))
			s.addConstraint(ClLinearInequality(ClLinearExpression(y), cnGEQ, ClLinearExpressionNum(0), clsRequired()))
			s.addConstraint(ClLinearInequality(ClLinearExpression(x), cnLEQ, ClLinearExpressionNum(MAXX), clsRequired()))
			s.addConstraint(ClLinearInequality(ClLinearExpression(y), cnLEQ, ClLinearExpressionNum(MAXY), clsRequired()))

		self._canvas.pack()

	def draw_first(self):
		# Lines
		for i in range(4):
			(x1, y1) = (self._points[i].Xvalue(),
				self._points[i].Yvalue())
			(x2, y2) = (self._points[(i+1)%4].Xvalue(),
				self._points[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")
		for i in range(4):
			(x1, y1) = (self._midpoints[i].Xvalue(),
				self._midpoints[i].Yvalue())
			(x2, y2) = (self._midpoints[(i+1)%4].Xvalue(),
				self._midpoints[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")

		# Points
		for p in self._points:
			# Create a dot for this point
			x = p.Xvalue()
			y = p.Yvalue()
			o = self._canvas.create_oval(x-DOTSIZE, y-DOTSIZE,
				x+DOTSIZE, y+DOTSIZE, fill='SkyBlue')
			# Bind functions to the points
			self._canvas.tag_bind(o, '<Any-Enter>', self.highlight)
			self._canvas.tag_bind(o, '<Any-Leave>', self.unhighlight)
			self._canvas.tag_bind(o, '<ButtonPress-1>',
				lambda e, s=self, p=p: s.btnDown(e, p))
			self._canvas.tag_bind(o, '<ButtonRelease-1>', self.btnUp)
			self._point_to_obj[p] = o
			self._oldloc[p] = (x, y)

			# Create a tag for this point
#			tagname = 'p%i' % self._points.index(p)
#			self._tags[p] = tagname
#			self._canvas.addtag_withtag(tagname, o)

		# Midpoints
		for p in self._midpoints:
			# Create a dot for this point
			x = p.Xvalue()
			y = p.Yvalue()
			o = self._canvas.create_oval(x-DOTSIZE, y-DOTSIZE,
				x+DOTSIZE, y+DOTSIZE, fill='Green')
			# Bind functions to this point
			self._canvas.tag_bind(o, '<Any-Enter>', self.highlight)
			self._canvas.tag_bind(o, '<Any-Leave>', self.unhighlight)
			self._canvas.tag_bind(o, '<ButtonPress-1>',
				lambda e, s=self, p=p: s.btnDown(e, p))
			self._canvas.tag_bind(o, '<ButtonRelease-1>', self.btnUp)
			self._point_to_obj[p] = o
			self._oldloc[p] = (x, y)

		self._canvas.bind('<B1-Motion>', self.btnMove)

	def draw_update(self):
		self._canvas.delete('lines')
		# Lines
		for i in range(4):
			(x1, y1) = (self._points[i].Xvalue(),
				self._points[i].Yvalue())
			(x2, y2) = (self._points[(i+1)%4].Xvalue(),
				self._points[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")
		for i in range(4):
			(x1, y1) = (self._midpoints[i].Xvalue(),
				self._midpoints[i].Yvalue())
			(x2, y2) = (self._midpoints[(i+1)%4].Xvalue(),
				self._midpoints[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")
		self._canvas.lower('lines', 'all')

		# Points
		for p in self._points:
			# Create a dot for this point
			(oldx, oldy) = self._oldloc[p]
			if (oldx != p.Xvalue()) or (oldy != p.Yvalue()):
				o = self._point_to_obj[p]
				x = p.Xvalue()
				y = p.Yvalue()
				self._canvas.move(o, (x-oldx), (y-oldy))
				self._oldloc[p] = (x, y)

		# Midpoints
		for p in self._midpoints:
			# Create a dot for this point
			x = p.Xvalue()
			y = p.Yvalue()
			(oldx, oldy) = self._oldloc[p]
			if (x != oldx) or (y != oldy):
				o = self._point_to_obj[p]
				self._canvas.move(o, (x-oldx), (y-oldy))
				self._oldloc[p] = (x, y)

	def highlight(self, e):
		self._oldcolor = self._canvas.itemcget('current', 'fill')
		self._canvas.itemconfig('current', fill='Red')
	def unhighlight(self, e):
		self._canvas.itemconfig('current', fill=self._oldcolor)

	def btnDown(self, event, point):
		# Edit constraints
		if self._editX:
			self._solver.removeConstraint(self._editX)
			self._editX = None
		if self._editY:
			self._solver.removeConstraint(self._editY)
			self._editY = None
		self._editX = ClEditConstraint(point.X(), clsStrong())
		self._editY = ClEditConstraint(point.Y(), clsStrong())
		self._solver.addConstraint(self._editX)
		self._solver.addConstraint(self._editY)

	def btnUp(self, event):
		if self._editX:
			self._solver.removeConstraint(self._editX)
			self._editX = None
		if self._editY:
			self._solver.removeConstraint(self._editY)
			self._editY = None
		self.draw_update()

	def btnMove(self, event):
		if self._editX:
			self._solver.resolveXY(event.x, event.y)
		self.draw_update()

def main():
	a = App()
	a.mainloop()

if __name__ == '__main__':
	main()
