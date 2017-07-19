#!/usr/bin/env python


import numpy as np
np.random.seed(12345)

import argparse

from keras.models import Sequential
from keras.layers import Dense

from ROOT import TFile, PyConfig
PyConfig.IgnoreCommandLineOptions = True

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input',
            help='Input ROOT file with training data "tmva_class_example.root"')
    parser.add_argument('--weights', default='weights.h5',
            help='Output weights file')
    parser.add_argument('--architecture', default='architecture.json',
            help='Output architecture file')
    return parser.parse_args()


def load_events(filename):
    # Open file
    file_ = TFile(filename)
    if file_ == None: raise Exception('Can not open file: {}'.format(filename))

    # Find trees
    tree_s = file_.Get('TreeS')
    if tree_s == None: raise Exception('Can not find tree: {}'.format('TreeS'))

    tree_b = file_.Get('TreeB')
    if tree_s == None: raise Exception('Can not find tree: {}'.format('TreeB'))

    # Get values
    events_s = np.zeros((tree_s.GetEntries(), 4))
    labels_s = np.ones((tree_s.GetEntries(), 2))

    events_b = np.zeros((tree_b.GetEntries(), 4))
    labels_b = np.ones((tree_b.GetEntries(), 2))

    for i, event in enumerate(tree_s):
        for j in range(4):
            events_s[i,j] = getattr(event, 'var{}'.format(j+1))

    for i, event in enumerate(tree_b):
        for j in range(4):
            events_b[i,j] = getattr(event, 'var{}'.format(j+1))

    return np.vstack((events_s, events_b)), np.vstack((labels_s, labels_b))


def build_model():
    model = Sequential()
    model.add(Dense(32, init='glorot_normal',
            activation='relu', input_dim=4))
    model.add(Dense(2, init='glorot_uniform',
            activation='softmax'))

    model.summary()

    model.compile(
            loss='categorical_crossentropy',
            optimizer='adam',
            metrics=['accuracy',])

    return model


def main(args):
    # Get training dataset
    events, labels = load_events(args.input)

    # Train model
    model = build_model()
    model.fit(events, labels, batch_size=100, nb_epoch=10, validation_split=0.5)

    # Save model with weights and architecture separated
    model.save_weights(args.weights, overwrite=True)

    file_ = open(args.architecture, 'w')
    file_.write(model.to_json())
    file_.close()


if __name__ == '__main__':
    args = parse_arguments()
    main(args)
