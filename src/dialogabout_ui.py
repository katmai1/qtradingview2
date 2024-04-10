# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'dialogabout.ui'
##
## Created by: Qt User Interface Compiler version 6.3.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QAbstractButton, QApplication, QDialog, QDialogButtonBox,
    QGroupBox, QSizePolicy, QVBoxLayout, QWidget)

class Ui_dialogAbout(object):
    def setupUi(self, dialogAbout):
        if not dialogAbout.objectName():
            dialogAbout.setObjectName(u"dialogAbout")
        dialogAbout.resize(320, 240)
        self.verticalLayout = QVBoxLayout(dialogAbout)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.groupBox = QGroupBox(dialogAbout)
        self.groupBox.setObjectName(u"groupBox")
        sizePolicy = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBox.sizePolicy().hasHeightForWidth())
        self.groupBox.setSizePolicy(sizePolicy)

        self.verticalLayout.addWidget(self.groupBox)

        self.buttonBox = QDialogButtonBox(dialogAbout)
        self.buttonBox.setObjectName(u"buttonBox")
        self.buttonBox.setOrientation(Qt.Horizontal)
        self.buttonBox.setStandardButtons(QDialogButtonBox.Close)
        self.buttonBox.setCenterButtons(True)

        self.verticalLayout.addWidget(self.buttonBox)


        self.retranslateUi(dialogAbout)
        self.buttonBox.accepted.connect(dialogAbout.accept)
        self.buttonBox.rejected.connect(dialogAbout.reject)

        QMetaObject.connectSlotsByName(dialogAbout)
    # setupUi

    def retranslateUi(self, dialogAbout):
        dialogAbout.setWindowTitle(QCoreApplication.translate("dialogAbout", u"Acerca de...", None))
        self.groupBox.setTitle(QCoreApplication.translate("dialogAbout", u"About", None))
    # retranslateUi

