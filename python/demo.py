#!/uns/bin/python

#
# QuadDemo example, using Cassowary/Python
# Tessa Lau, tlau@cs.washington.edu, 2/23/98
#

from cassowary import *
from Tkinter import *

DOTSIZE = 5			# Size of dot to draw
MAXX = MAXY = 450		# Max size of window

class App(Frame):
	def __init__(self, parent=None):
		# Widget initialization
		Frame.__init__(self)
		self.pack()
		self._canvas = Canvas(width=MAXX, height=MAXY)
		self._canvas.pack()

		# Local vars
		self._editX = None		# Edit constraint on X
		self._editY = None		# Edit constraint on Y
		self._oldloc = {}		# Mapping from point to (x,y) location
		self._point_to_obj = {}	# Mapping from point to canvas obj

		# Create constraints, and refresh
		self.create_constraints()
		self.draw_first()

	def create_constraints(self):
		coords = [(50,50),(50,250),(250,250),(250,50)]
		self._p = map(
			lambda (x, y): ClPoint(x, y),
			coords)
		self._mp = map(lambda x: ClPoint(), coords)

		s = self._solver = ClSimplexSolver()
		weight = 0.5
		for p in self._p:
			s.AddPointStay(p, weight)
			weight = weight * 2

		# Midpoints
		for i in range(4):
			a = i
			b = (i+1) % 4
			s.AddConstraint(ClLinearEquation(self._mp[a].X(),
				(self._p[b].X() + self._p[a].X())/2))
			s.AddConstraint(ClLinearEquation(self._mp[a].Y(),
				(self._p[b].Y() + self._p[a].Y())/2))

		# Keep from turning inside out
		for (p1, p2) in [(0, 2), (0, 3), (1, 2), (1, 3)]:
			c = ClLinearInequality(self._p[p1].X() + DOTSIZE,
				cnLEQ, self._p[p2].X())
			s.AddConstraint(c)
		for (p1, p2) in [(0, 1), (0, 2), (3, 1), (3, 2)]:
			c = ClLinearInequality(self._p[p1].Y() + DOTSIZE,
				cnLEQ, self._p[p2].Y())
			s.AddConstraint(c)

		# Boundaries
		for p in self._p:
			x = p.X()
			y = p.Y()
			s.AddConstraint(ClLinearInequality(x, cnGEQ, DOTSIZE))
			s.AddConstraint(ClLinearInequality(y, cnGEQ, DOTSIZE))
			s.AddConstraint(ClLinearInequality(x, cnLEQ, MAXX-DOTSIZE))
			s.AddConstraint(ClLinearInequality(y, cnLEQ, MAXY-DOTSIZE))

		self._canvas.pack()

	def draw_first(self):
		# Lines
		for i in range(4):
			(x1, y1) = (self._p[i].Xvalue(),
				self._p[i].Yvalue())
			(x2, y2) = (self._p[(i+1)%4].Xvalue(),
				self._p[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")
		for i in range(4):
			(x1, y1) = (self._mp[i].Xvalue(),
				self._mp[i].Yvalue())
			(x2, y2) = (self._mp[(i+1)%4].Xvalue(),
				self._mp[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")

		# Draw points
		for p in self._p:
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

		# Draw midpoints
		for p in self._mp:
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
		# Draw lines
		for i in range(4):
			(x1, y1) = (self._p[i].Xvalue(),
				self._p[i].Yvalue())
			(x2, y2) = (self._p[(i+1)%4].Xvalue(),
				self._p[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")
		for i in range(4):
			(x1, y1) = (self._mp[i].Xvalue(),
				self._mp[i].Yvalue())
			(x2, y2) = (self._mp[(i+1)%4].Xvalue(),
				self._mp[(i+1)%4].Yvalue())
			self._canvas.create_line(x1, y1, x2, y2, tags="lines")
		self._canvas.lower('lines', 'all')

		# Points
		for p in self._p:
			# Create a dot for this point
			(oldx, oldy) = self._oldloc[p]
			if (oldx != p.Xvalue()) or (oldy != p.Yvalue()):
				o = self._point_to_obj[p]
				x = p.Xvalue()
				y = p.Yvalue()
				self._canvas.move(o, (x-oldx), (y-oldy))
				self._oldloc[p] = (x, y)

		# Midpoints
		for p in self._mp:
			# Create a dot for this point
			x = p.Xvalue()
			y = p.Yvalue()
			(oldx, oldy) = self._oldloc[p]
			if (x != oldx) or (y != oldy):
				o = self._point_to_obj[p]
				self._canvas.move(o, (x-oldx), (y-oldy))
				self._oldloc[p] = (x, y)

	# Recolor point when mouse enters
	def highlight(self, e):
		self._oldcolor = self._canvas.itemcget('current', 'fill')
		self._canvas.itemconfig('current', fill='Red')
	# Recolor point when mouse leaves
	def unhighlight(self, e):
		self._canvas.itemconfig('current', fill=self._oldcolor)

	# On button-down, Add edit constraints on this point
	def btnDown(self, event, point):
		# Edit constraints
		if self._editX:
			self._solver.RemoveConstraint(self._editX)
			self._editX = None
		if self._editY:
			self._solver.RemoveConstraint(self._editY)
			self._editY = None
		self._editX = ClEditConstraint(point.X())
		self._editY = ClEditConstraint(point.Y())
		self._solver.AddConstraint(self._editX)
		self._solver.AddConstraint(self._editY)

	# On button-up, remove edit constraints
	def btnUp(self, event):
		if self._editX:
			self._solver.RemoveConstraint(self._editX)
			self._editX = None
		if self._editY:
			self._solver.RemoveConstraint(self._editY)
			self._editY = None
		self.draw_update()

	# On mouse motion, if edit constraints are registered: Resolve
	def btnMove(self, event):
		if self._editX:
			self._solver.Resolve(event.x, event.y)
		self.draw_update()

def main():
	a = App()
	a.mainloop()

if __name__ == '__main__':
	main()
