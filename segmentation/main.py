from model import *
from data import *

#os.environ["CUDA_VISIBLE_DEVICES"] = "0"


data_gen_args = dict(rotation_range=0.2,
                    width_shift_range=0.05,
                    height_shift_range=0.05,
                    shear_range=0.05,
                    zoom_range=0.05,
                    horizontal_flip=True,
                    fill_mode='nearest')
"""
myGene = trainGenerator(2,'data/Cars/train','image','label',data_gen_args,save_to_dir = None)
model = unet()
model_checkpoint = ModelCheckpoint('unet_car.hdf5', monitor='loss',verbose=1, save_best_only=True)
model.fit_generator(myGene,steps_per_epoch=20,epochs=5,callbacks=[model_checkpoint])
"""

model = unet('unet_car.hdf5')
#model = unet('15_5_1000.hdf5')

testGene = testGenerator("data/Cars/test")
results = model.predict_generator(testGene,21,verbose=1)
saveResult("data/Predict",results)

from PIL import Image
import matplotlib.pyplot as plt
img = Image.open("data/Cars/test/17.png")
image = img.resize((256,256))
plt.imshow(np.asarray(image))
plt.imshow(np.squeeze(results[17]),alpha=0.75)

plt.show()

