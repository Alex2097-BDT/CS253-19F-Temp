# -*- coding=utf-8 -*-
import numpy as np
import numpy.matlib
import sys
import string
import random

trans_mat_c = {
    'a': ['4', '/-\\', '/_\\', '@', '/\\'],
    'b': ['8', '|3', '13', '|}', '|:', '|8', '18', '6', '|B', 'lo', '|o', 'j3', 'ß'],
    'c': ['<', '{', '[', '(', '©', '¢'],
    'd': ['|)', '|}', '|]', '|>'],
    'e': ['3', '£', '₤', '€'],
    'f': ['|=', 'ph', '|#', '|"'],
    'g': ['[', '-', '[+', '6', 'C-'],
    'h': ['4', '|-|', '[-]', '{-}', '}-{', '}{', '|=|', '[=]', '{=}', '/-/', '(-)', ')-(', ':-:', 'I+I'],
    'i': ['1', '|', '!', '9'],
    'j': ['_|', '_/', '_7', '_)', '_]', '_}'],
    'k': ['|<', '1<', 'l<', '|{', 'l{'],
    'l': ['|_', '|', '1', ']['],
    'm': ['44', '|\/|', '^^', '/\/\\', '/X\\', '[]\/][', '[]V[]', '][\\\\//][', '(V)', '//.', '.\\\\', 'N\\'],
    'n': ['|\|', '/\/', '/V', '][\\\\][', 'И'],
    'o': ['0', '()', '[]', '{}', '<>', 'Ø', 'oh'],
    'p': ['|o', '|O', '|>', '|*', '|°', '|D', '/o', '[]D', '|7'],
    'q': ['O_', '9', '(', ')', '0', 'kw'],
    'r': ['|2', '12', '.-', '|^', 'l2', 'Я', '®'],
    's': ['5', '$', '§'],
    't': ['7', '+', '7`', "'|'", '`|`', '~|~', '-|-', "']['"],
    'u': ['|_|', '\_\\', '/_/', '\_/', '(_)', '[_]', '{_}'],
    'v': ['\/'],
    'w': ['\/\/', '(/\)', '\^/', '|/\|', '\X/', "\\\\'", "'//", 'VV', '\_|_/', '\\\\//\\\\//', 'Ш', '2u', '\V/'],
    'x': ['%', '*', '><', '}{', ')(', 'Ж'],
    'y': ['`/', '¥', '\|/', 'Ч'],
    'z': ['2', '5', '7', '>_', '(/)'],
    '0': ['O', 'D'],
    '1': ['|', 'L'],
    '2': ['Z', 'e'],
    '3': ['E', 'R'],
    '4': ['h', 'A'],
    '5': ['S'],
    '6': ['b', 'G'],
    '7': ['T', 'j'],
    '8': ['B', 'X'],
    '9': ['g', 'J'],
    ' ': [' ']
}


def leet_char(ch):
    chs = trans_mat_c[ch]
    return random.choice(chs)


def leet_word(word):
    return ''.join([leet_char(ch) for ch in word])


def get_words(file):
    space_item = [' ']
    li = list(open(file).read())
    np_li = np.array(space_item + li + space_item)
    np_li[~np.char.isalpha(np_li)] = ' '
    np_li = np.char.lower(np_li)
    space_indices_end = np.where(np_li == ' ')[0]
    space_indices_start = space_indices_end[0:-1]
    space_indices_end = space_indices_end[1:]
    space_indices_pair = np.array([space_indices_start, space_indices_end])
    space_indices_mat = np.asmatrix(space_indices_pair).T
    space_indices_pair = np.asarray(space_indices_mat)
    space_indices_pair = space_indices_pair[np.where(space_indices_pair[:, 1]-space_indices_pair[:, 0] > 1)]
    ret = np.array([''.join(word_item).strip() for word_item in [np_li[space_indices_pair[i][0]: space_indices_pair[i][1]] for i in range(len(space_indices_pair))]])
    return ret


if __name__ == '__main__':
    words_1 = get_words(sys.argv[1])
    words_2 = words_1[1:]
    words_1 = words_1[0:-1]
    words_pair = np.array([words_1, words_2])
    words_mat = np.asmatrix(words_pair).T
    words_pair = np.asarray(words_mat)
    if len(sys.argv) == 3 and sys.argv[2] == "-allow-stop-words":
        words_pair = [item[0] + ' ' + item[1] for item in words_pair]
    else:
        swf = open('./stop_words.txt').read().split(',')
        swf.append('s')
        swf.append('t')
        swf.append('d')
        words_pair = [item[0] + ' ' + item[1] for item in words_pair if item[0] not in swf and item[1] not in swf]
    u, c = np.unique(words_pair, axis=0, return_counts=True)
    two_gram_sorted = sorted(zip(u, c), key=lambda k: k[1], reverse=True)[0:5]
    for t, c in two_gram_sorted:
        print(t, ' - ', leet_word(t), ' - ', c)
