from keras.applications.inception_resnet_v2 import InceptionResNetV2
#Load the ... model
###vgg_conv = irn2
irn2 = InceptionResNetV2(include_top=False,
                             weights='imagenet',
                             input_tensor=None,
                             input_shape=(299,299,3),
                             pooling=None,
                             classes=1000)
for layer in irn2.layers[:-18]:
    layer.trainable = False
#for layer in irn2.layers:
 #   print(layer,layer.trainable)

from keras import models
from keras import layers
from keras import optimizers

model = models.Sequential()
model.add(irn2)

model.add(layers.Flatten())
model.add(layers.Dense(1024,activation='relu'))
model.add(layers.Dropout(0.5))
model.add(layers.Dense(3,activation='softmax'))
#model.summary()
train_dir='images_train'
valid_dir='images_valid'
from keras.preprocessing.image import ImageDataGenerator

train_datagen = ImageDataGenerator(
    rescale=1./255,
    rotation_range=20,
    width_shift_range=0.2,
    height_shift_range=0.2,
    horizontal_flip=True,
    fill_mode='nearest')
valid_datagen = ImageDataGenerator(rescale=1./255)
train_batchsize = 100
valid_batchsize = 10

train_gen = train_datagen.flow_from_directory(
    train_dir,
    target_size = (299,299),
    batch_size=train_batchsize,
    class_mode='categorical')

valid_gen = valid_datagen.flow_from_directory(
    valid_dir,
    target_size=(299,299),
    batch_size=valid_batchsize,
    class_mode='categorical',
    shuffle=False)

model.compile(loss='categorical_crossentropy',
              optimizer=optimizers.RMSprop(lr=1e-4),
              metrics=['acc'])
"""
history = model.fit_generator(
    train_gen,
    steps_per_epoch = train_gen.samples/train_gen.batch_size ,
    epochs=2,
    validation_data = valid_gen,
    validation_steps = valid_gen.samples/valid_gen.batch_size,
    verbose=1)
"""
history = model.fit_generator(train_gen,
                              steps_per_epoch = train_gen.samples/train_gen.batch_size,
                              validation_data = valid_gen,
                              validation_steps = valid_gen.samples/valid_gen.batch_size,
                              epochs=18,
                              verbose=1,
                              workers=1,
                              use_multiprocessing=False)

model.save('lastFour.h5')
