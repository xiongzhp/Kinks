#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
from array import array

gapchars = "-/"


def map_gaps(seq):
  ungapped2gapped = []
  gapped2ungapped = []
  n=0
  for i, c in enumerate(seq):
    gapped2ungapped.append(n)
    if not isGap(c):
      ungapped2gapped.append(i)
      n+=1
  return ungapped2gapped, gapped2ungapped


def getGapChars():
  return gapchars

def isGap(c):
  return c in gapchars


def seqId(seq1, seq2):
    assert len(seq1) == len(seq2)
    assert len(seq1) > 0
    same=0
    nongap=0
    for i in xrange(len(seq1)):
        c1 = seq1[i]
        c2 = seq2[i]
        if c1 in gapchars or c2 in gapchars:
            continue
        nongap+=1
        if c1==c2:
            same+=1
    if nongap<1:
      return 0.0
    return float(same)/nongap


def deGappify(seq):
    output = array('c')
    for c in seq:
        if c not in gapchars:
            output.append(c)
    return output.tostring()


def count_gaps(seq, start=0, end=None):
    if end is None:
      end = len(seq)
    
    n = 0
    for c in seq[start:end]:
        if isGap(c):
            n += 1
    return n


def count_nongaps(seq):
    return len(seq) - count_gaps(seq)


def findAllGaps(seq):
    indeces = []
    for i,c in enumerate(seq):
      if c in gapchars:
        indeces.append(i)
    return indeces



def equals_ungapped(a, b):
    i=0
    try:
      for c in a:
        if c in gapchars:
          continue
        while b[i] in gapchars:
          i += 1
        if c != b[i]:
          return False
        i += 1
    except IndexError:
      return False
    
    while i < len(b):
      if b[i] not in gapchars:
        return False
      i += 1
    
    return True


def length_ungapped(seq):
    return len(seq) - count_gaps(seq)


def gappify(template, target):
    #assert(type(template) == str), "Illegal argument type. Must be type 'str'."
    #assert(type(target) == str), "Illegal argument type. Must be type 'str'."
    
    target = deGappify(target)
    
    n=0
    i=0
    template_len = len(template)
    target_len   = len(target)
    
    if target_len > template_len:
        #sys.stderr.write("ERROR: Target longer than template: "+str(target_len)+" > "+str(template_len)+"\n")
        #return ''
        raise ValueError("ERROR: Target longer than template: "+str(target_len)+" > "+str(template_len))
    
    output = array('c', template)
    
    try:
        while i < template_len:
            if not isGap(output[i]):
               output[i] = target[n]
               n+=1
            i+=1
    except IndexError:
        #sys.stderr.write("ERROR: Template too long for target sequence: n > "+str(target_len)+"\n")
        ##sys.stderr.write("TEMPLATE:\n"+template+"\n")
        ##sys.stderr.write("TARGET:\n"+target+"\n")
        #return ''
        raise ValueError("ERROR: Template too long for target sequence: n > "+str(target_len))
    
    if n != target_len:
        #sys.stderr.write("ERROR: Template too short for target sequence: "+str(n)+" < "+str(target_len)+"\n")
        ##sys.stderr.write("TEMPLATE:\n"+template+"\n")
        ##sys.stderr.write("TARGET:\n"+target+"\n")
        #return ''
        raise ValueError("ERROR: Template too short for target sequence: "+str(n)+" < "+str(target_len))
    
    return output.tostring()


def gappifyList(template, target, gapObject=None, replaceGaps=True):
    #assert(type(template) == str), "Illegal argument type. Must be type 'str'."
    #assert(type(target) == str), "Illegal argument type. Must be type 'str'."
    
    n=0
    i=0
    template_len = len(template)
    target_len   = len(target)
    
    if target_len > template_len:
        sys.stderr.write("ERROR: Target longer than template: "+str(target_len)+" > "+str(template_len)+"\n")
        return ''
    
    output = list(template)
    
    try:
        while i < template_len:
            if output[i] not in gapchars:
                output[i] = target[n]
                n+=1
            elif replaceGaps:
                output[i]=gapObject
            i+=1
    except IndexError:
        sys.stderr.write("ERROR: Template too long for target sequence: n > "+str(target_len)+"\n")
        #sys.stderr.write("TEMPLATE:\n"+template+"\n")
        #sys.stderr.write("TARGET:\n"+target+"\n")
        return ''
    
    if n != target_len:
        sys.stderr.write("ERROR: Template too short for target sequence: "+str(n)+" < "+str(target_len)+"\n")
        #sys.stderr.write("TEMPLATE:\n"+template+"\n")
        #sys.stderr.write("TARGET:\n"+target+"\n")
        return ''
    
    return output


def gappifyDebug(template, target, verbose=True):
    overlap = None
    error = None
    
    n=0
    i=0
    
    template_len = len(template)
    target_len   = len(target)
    
    gaps=0
    for c in template:
        if c in gapchars:
            gaps+=1
    
    ungapped_len = template_len-gaps
    
    if target_len != ungapped_len:
        error = "ERROR: Target length ("+str(target_len)+") != ungapped template lenght ("+str(template_len)+"-"+str(gaps)+"="+str(ungapped_len)+")"
    
    overlap = array('c', template)
    try:
        while i < template_len:
            if overlap[i] not in gapchars:
                overlap[i] = target[n]
                n+=1
            i+=1
    except IndexError:
        pass
    
    
    match             = overlap[0:i].tostring()
    template_overhang = template[i:]
    target_overhang   = target[n:]
    
    if verbose and error:
        bla=''
        bla+= "GAPPIFY:"
        bla+= "\t"   + "Error" + "\n"
        bla+= "\t\t" + error + "\n"
        bla+= "\t"   + "Match" + "\n"
        bla+= "\t\t" + match + "\n"
        bla+= "\t"   + "Template overhang" + "\n"
        bla+= "\t\t" + template_overhang + "\n"
        bla+= "\t"   + "Target overhang" + "\n"
        bla+= "\t\t" + target_overhang + "\n"
        bla+= "\n"
        sys.stderr.write(bla)
    
    return match, template_overhang, target_overhang, error

class GappySequenceError(ValueError):
  pass
