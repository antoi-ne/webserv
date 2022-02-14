class	File:
	def __init__(self, file_path):
		self.file_name = self.__getFilename(file_path)
		self.content = self.__readFile(file_path)

	def __getFilename(self, file_path):
		start = file_path.rfind('/')
		if start == -1:
			start = 0
		return file_path[start:]

	def __readFile(self, file_path):
		with open(file_path, "rb") as file:
			_file = file.read()
		return _file