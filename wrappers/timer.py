from time import *

class Timer:
	def __init__(self):
		self._start = 0
		self._elapsed = 0
		self._isrunning = 0

	def Start(self):
		self._isrunning = 1
		self._start = clock()

	def Stop(self):
		self._isrunning = 0
		self._elapsed = self._elapsed + clock() - self._start
	
	def Reset(self):
		self._isrunning = 0
		self._elapsed = 0
	
	def IsRunning(self):
		return self._isrunning
	
	def ElapsedTime(self):
		if not self._isrunning:
			return self._elapsed
		else:
			return self._elapsed + clock() - self._start

	def Resolution(self):
		pass

