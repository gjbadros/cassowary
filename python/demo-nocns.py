#!/uns/bin/python

#
# The QuadDemo without constraints (for speed comparison)
# ... basically there's hardly any overhead for using the constraints ...
#

from Tkinter import *

DOTSIZE = 5
MAXX = MAXY = 450

class Point:
	def __init__(self, x, y):
		self._x = x
		self._y = y

	def Xvalue(self):
		return self._x

	def Yvalue(self):
		return self._y

	def setX(self, x):
		self._x = x
	def setY(self, y):
		self._y = y

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

		self.create_constraints()
		self.draw_first()

	def create_constraints(self):
		coords = [(50,50),(50,250),(250,250),(250,50)]
		self._p = map(
			lambda (x, y): Point(x, y), coords)
		mp_coords = [(50, 150), (150, 250), (250, 150), (150, 50)]
		self._mp = map(lambda (x, y): Point(x, y), mp_coords)

		self._canvas.pack()

	def draw_first(self):
		# Lines
		for i in range(4):
			(x1, y1) = (self._p[i].Xvalue(), self._p[i].Yvalue())
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
		self._curpoint = point
		pass

	# On button-up, remove edit constraints
	def btnUp(self, event):
		self._curpoint = None
		if self._editX:
			self._solver.RemoveConstraint(self._editX)
			self._editX = None
		if self._editY:
			self._solver.RemoveConstraint(self._editY)
			self._editY = None
		self.draw_update()

	# On mouse motion, if edit constraints are registered: Resolve
	def btnMove(self, event):
		if self._curpoint:
			self._curpoint.setX(event.x)
			self._curpoint.setY(event.y)
		self.draw_update()

def main():
	a = App()
	a.mainloop()

if __name__ == '__main__':
	main()
