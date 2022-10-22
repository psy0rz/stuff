import numpy as np
import cv2 as cv
import cv2
from matplotlib import pyplot as plt



cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()
while True:
    # Capture frame-by-frame
    ret, img = cap.read()
    # cv2.imshow('cam', img)
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    # gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Display the resulting frame
    # cv.imshow('frame', gray)



    edges = cv.Canny(img, 100,200)

    contours, hierarcy=cv.findContours(edges, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_SIMPLE)
    # cv2.drawContours(image=img, contours=contours, contourIdx=-1, color=(255,255,255), thickness=1,
    #                  lineType=cv2.LINE_AA)


    for contour in contours:

        rect = cv.minAreaRect(contour)

        centerPt=( int(rect[0][0]) , int(rect[0][1]))
        w=int(rect[1][0])
        h=int(rect[1][1])

        # cv.drawMarker(img,centerPt , color=(255,0,0))

        detectSize=80

        if ((abs(detectSize-w)<10) and abs(detectSize-h)<10):

            cv.putText(img , "{}x{}".format(w,h), centerPt, fontFace=cv.FONT_HERSHEY_COMPLEX, fontScale=0.5, color=(0,0,255))

            box = cv.boxPoints(rect)


            box = np.int0(box)

            cv.drawContours(img, [box], 0, (255, 0, 0), 2)
            t = np.int0(box)



        # cv.drawContours(img, [[rect[0][0], rect[0][1], rect[1][0], rect[1][1]]], 0, (255, 0, 0), 2)

    # print(edges[0])

    cv2.imshow('cam', img)
    if cv.waitKey(100) == ord('q'):
        break


# When everything done, release the capture
cap.release()
cv.destroyAllWindows()
