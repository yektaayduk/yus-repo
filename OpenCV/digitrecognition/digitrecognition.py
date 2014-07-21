
import cv2
import numpy as np
from PIL import Image, ImageFont, ImageDraw, ImageOps

# create training model based on the given TTF font file
def createDigitsModel(fontfile, digitheight):
    ttfont = ImageFont.truetype(fontfile, digitheight)
    samples =  np.empty((0,digitheight*(digitheight/2)))
    responses = []
    for n in range(10):
        pil_im = Image.new("RGB", (digitheight, digitheight*2))
        ImageDraw.Draw(pil_im).text((0, 0), str(n), font=ttfont)
        pil_im = pil_im.crop(pil_im.getbbox())
        pil_im = ImageOps.invert(pil_im)
        #pil_im.save(str(n) + ".png")

        # convert to cv image
        cv_image = cv2.cvtColor(np.array( pil_im ), cv2.COLOR_RGBA2BGRA)
        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray,(5,5),0)
        thresh = cv2.adaptiveThreshold(blur,255,1,1,11,2)

        roi = cv2.resize(thresh,(digitheight,digitheight/2))
        responses.append( n )
        sample = roi.reshape((1,digitheight*(digitheight/2)))
        samples = np.append(samples,sample,0)

    samples = np.array(samples,np.float32)
    responses = np.array(responses,np.float32)
    
    model = cv2.KNearest()
    model.train(samples,responses)
    return model

# digit recognition part
def findDigits(imagefile, digitheight, fontfile="C:\\Windows\\Fonts\\Arial.ttf"):
    im = cv2.imread(imagefile)
    out = np.zeros(im.shape,np.uint8)
    gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    thresh = cv2.adaptiveThreshold(gray,255,1,1,11,2)

    contours,hierarchy = cv2.findContours(thresh.copy(),cv2.RETR_EXTERNAL ,cv2.CHAIN_APPROX_SIMPLE)

    model = createDigitsModel(fontfile, digitheight)
    for cnt in contours:
        x,y,w,h = cv2.boundingRect(cnt)
        if  h>w and h>(digitheight*4)/5 and h<(digitheight*6)/5: #+/-20%
            cv2.rectangle(im,(x,y),(x+w,y+h),(0,255,0),1)
            roi = thresh[y:y+h,x:x+w] # crop
            roi = cv2.resize(roi,(digitheight,digitheight/2))
            roi = roi.reshape((1,digitheight*(digitheight/2)))
            roi = np.float32(roi)
            retval, results, neigh_resp, dists = model.find_nearest(roi, k=1)
            string = str(int((results[0][0])))
            #cv2.drawContours(out,[cnt],-1,(0,255,255),1)
            cv2.putText(out,string,(x,y+h),0,1,(0,255,0))

    cv2.imshow('in',im)
    cv2.imshow('out',out)
    cv2.waitKey(0)
    cv2.destroyWindow( 'in' )
    cv2.destroyWindow( 'out' )

findDigits('pi.png', 32)
print 'done.'
