#!/usr/bin/env python


import ROOT
from array import array


file_input = ROOT.TFile('tmva_class_example.root')
tree_input = file_input.Get('TreeS')

file_output = ROOT.TFile('tmva_class_example_large.root', 'RECREATE')
tree_output = ROOT.TTree('TreeS', 'TreeS')
variables = {}
names = ['var1', 'var2', 'var3', 'var4']
for name in names:
    variables[name] = array('f', [-999])
    tree_output.Branch(name, variables[name], name+'/F')

for x in range(int(1000000/tree_input.GetEntries())):
    for event in tree_input:
        for name in names:
            variables[name][0] = getattr(event, name)
        tree_output.Fill()

file_output.Write()
file_output.Close()
file_input.Close()
