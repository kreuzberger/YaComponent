
from PySide2.QtCore import QThread

class Thread(QThread):

    def run(self):
        QThread.run(self)

