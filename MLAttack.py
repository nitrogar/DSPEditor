from keras.models import Sequential 
from keras.layers import Activation, Dense ,BatchNormalization
from keras import initializers 
from keras import regularizers 
from keras import constraints
from keras.utils import text_dataset_from_directory
import numpy as np

class MLAttack:

    def __init__(self,train_group,test_group,model) -> None:
        self.train_group = train_group
        self.test_group  = test_group
        self.model       = model
        self.prediction_error = 0
        self.mispredicted_traces = None
        self.epochs = 1000
        self.verbose = True

    def train(self):
       


    
        train_x,train_y = self.train_group.get_training_data()
        self.model.compile(optimizer = 'adam',
              loss = 'sparse_categorical_crossentropy',
              metrics = ['accuracy'])

        self.model.summary()
        self.log(f"trining x : {train_x.shape} train_y: {train_y.shape}")
        history = self.model.fit(train_x, train_y, epochs=self.epochs)

    def set_epochs(self,ep):
        self.epochs = ep
    def test(self):
        test_x,test_y = self.test_group.get_training_data()
        error = 0
        number_of_tests = len(test_x)
        size = len(test_x[0])
        self.log(f"test_x: {test_x.shape} test_y: {test_y.shape}")
    
        for i in range(number_of_tests):
            test_i = np.zeros((1,size), dtype=np.float64)
            test_i[0] = test_x[i]
            predict_y = self.model.predict(test_i ,verbose=1)
            predicted_class =np.argmax(predict_y,axis=1)
            self.log(f"{np.flip(np.argsort(predict_y,axis=1)[0])[:5]}")
            self.log(f"{-np.sort(-predict_y)[0][0:5]}")
            if predicted_class != test_y[i] :
                self.log(f"[{error}] mispredict {predicted_class} != {test_y[i]}")
                error += 1
        return (error / number_of_tests) * 100
    def log(self,msg):
        if self.verbose :
            print(msg)

