import re, sys, operator

RECURSION_LIMIT = 9500

sys.setrecursionlimit(RECURSION_LIMIT+10)

stop_words = set(open('./stop_words.txt').read().split(','))
words = re.findall('[a-z]{2,}', open(sys.argv[1]).read().lower())
word_freqs = {}

# Y-Combinator
Y = (lambda h: lambda F: F(lambda x: h(h)(F)(x)))(lambda h: lambda F: F(lambda x: h(h)(F)(x)))

for i in range(0, len(words), RECURSION_LIMIT):
    # function count
    Y(lambda _count: lambda _word_list: lambda _stop_words: lambda _word_freqs: None if _word_list == []
        else _count(_word_list[1:])(_stop_words)(_word_freqs)
            if _word_freqs.update({_word_list[0]: _word_freqs.get(_word_list[0], 0) + (0 if _word_list[0] in _stop_words else 1)}) is None
            else 1
      )(words[i:i+RECURSION_LIMIT])(stop_words)(word_freqs)

# function wf_print
Y(lambda _wf_print: lambda _word_freqs: None if _word_freqs == []
        else _wf_print(_word_freqs[1:]) if print(_word_freqs[0][0] + " - " + str(_word_freqs[0][1])) is None
        else 0
  )(sorted(word_freqs.items(), key=operator.itemgetter(1), reverse=True)[:25])
