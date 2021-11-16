
import cv2

cam=cv2.VideoCapture(0)

while True:
    (ret , frame)=cam.read()

    if not ret:
        break

    cv2.imshow("cam", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    imgray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(imgray,58,255,0)

    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    cimg = cv2.drawContours(frame, contours, -1, (0, 255, 0), 1)
    cv2.imshow("moi", cimg)





