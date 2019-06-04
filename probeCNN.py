from keras.applications.inception_resnet_v2 import InceptionResNetV2
from keras.preprocessing import image
from keras.applications.inception_resnet_v2 import preprocess_input
from keras.applications.inception_resnet_v2 import decode_predictions
import numpy as np

model = InceptionResNetV2(weights='imagenet')

#image_path = 'elephant.jpg'
#image_path = 'muppet2.jpg'
#image_path = 'algo.jpg'
#image_path = 'gioconda.jpg'
image_path = 'perros.jpg'

img = image.load_img(image_path,target_size=(299,299))

x=image.img_to_array(img)
x=np.expand_dims(x,axis=0)
x=preprocess_input(x)

preds = model.predict(x)

for index, res in enumerate(decode_predictions(preds,top=10)[0]):
    print('{}. \t {}\t:{:.3f}%'.format(index +1, res[1],100*res[2]))
