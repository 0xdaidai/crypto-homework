from base import *
import itertools
import enchant

dic_checked = {}


def initial(text: str):
    frequency = [0] * 26
    order = []
    no_letter = []

    for each in "!\"#$%&()*+,./:;<=>?@[\\]^_{}|~-'’‘":
        text = text.replace(each, " ")

    text = text.lower()

    for c in text:
        if 'a' <= c <= 'z':
            frequency[count(c)] += 1

    word_list = text.split()

    for i in range(26):
        if frequency[i] == 0:
            no_letter.append(letter(i))

    for i in range(26):
        c_max = max(frequency)
        if c_max == 0:
            order += no_letter
            break
        else:
            c = frequency.index(c_max)
            order.append(letter(c))
            frequency[c] = 0

    return order, word_list


def dic_fine(letter: str, dictionary=None):
    if dictionary is None:
        dic = open(letter + ".txt", "r").read().splitlines()
    else:
        dic = []
        for word in dictionary:
            if letter in word and word not in dic:
                dic.append(word)
    return dic


def dic_check_1(dic: list, length: int):
    dic_checked_1 = []
    global dic_checked #设计成全局变量使得后来的字母对应关系不会影响前面的
    dic_checked = {}
    if length in dic_checked:
        return dic_checked[length]
    else:
        for word in dic:
            if len(word) == length and word not in dic_checked_1: #找出同长度单词
                dic_checked_1.append(word)
        dic_checked[length] = dic_checked_1
        return dic_checked_1


def word_check_1(word_to_be_check: str, word: str, letter_word1: str, letter_word2: str):
#检测两个单词中同等级（字频）的字母的相对位置是否一样
    l = len(word)
    for i in range(l):
        if word[i] == letter_word2:
            if word_to_be_check[i] != letter_word1:
                return False
        if word_to_be_check[i] == letter_word1:
            if word[i] != letter_word2:
                return False
    return True


def word_check_2(word_to_be_check: str, word: str, letter1: str, letter2: str):
#检测该单词对能否正确产生一个对应关系
    dic_check = {}
    l = len(word)
    for i in range(l):
        if word[i] == letter2:
            if word_to_be_check[i] != letter1:
                return False
        else:
            if word[i] in dic_check:
                if dic_check.get(word[i]) != word_to_be_check[i]:
                    return False
            else:
                dic_check[word[i]] = word_to_be_check[i]
    return True


def dic_check_2(dic: list, check_word: str, letter_word1: str, letter_word2: str):
#生成特定字母能产生对应的单词列表
    dic_checked_2 = []
    for word in dic:
        if word_check_1(word, check_word, letter_word1, letter_word2):
            dic_checked_2.append(word)
    return dic_checked_2


def dic_check_3(dic: list, check_word: str, letter_word1: str, letter_word2: str):
#生成单词对能产生单射关系的单词列表
    dic_checked_3 = []
    for word in dic:
        if word_check_2(word, check_word, letter_word1, letter_word2):
            dic_checked_3.append(word)
    return dic_checked_3


def analyse(dictionary: list, word_list: list, letter1: str, letter2: str):
    dic_letter1 = dic_fine(letter1, dictionary=dictionary)#包含需求字母的单词（字典中）
    print("letter1_fined")
    dic_letter2 = dic_fine(letter2, dictionary=word_list)#包含同字频字母的单词（文章中）
    print("letter2_fined")
    dic = {}
    for word in dic_letter2:
        print(word, ':')
        possi_list = dic_check_1(dic_letter1, len(word))
        print("checked_1")
        possi_list = dic_check_2(possi_list, word, letter1, letter2)
        print("checked_2")
        possi_list = dic_check_3(possi_list, word, letter1, letter2)
        print("checked_3")
        dic[word] = possi_list
        if len(possi_list) == 0:
            return False
    return dic #成功由word生成一个对应关系


def add_letter(dic: dict, word1: str, word2: str):
#由单词对补充映射
    l = len(word1)
    for i in range(l):
        if word1[i] not in dic:
            dic[word1[i]] = word2[i]


def check_letter(dic: dict, word1: str, word2: str):
#检查该单词对能否满足映射表已有的映射
    l = len(word1)
    for i in range(l):
        if word1[i] in dic:
            if dic[word1[i]] != word2[i]:
                return False
    return True


def is_finish(word_dic: dict):
#检查word_dic中是否还有单词
    for word in word_dic:
        if len(word_dic.get(word)) > 1:
            return False
    return True


def analyse_dic(dic: dict, word_dic: dict, word: str):
    dic_copy = dic
    dic = dic_copy.copy()
    word_dic = word_dic.copy()
    add_letter(dic, word, word_dic.get(word)[0])
    flag_pre = 0
    while not is_finish(word_dic):
        for word in word_dic:
            word_check_list = word_dic.get(word)
            if len(word_check_list) != 1:
                for word_check in word_check_list:
                    if not check_letter(dic, word, word_check):
                        word_check_list.remove(word_check)
        flag = 0
        for word in word_dic:
            if len(word_dic.get(word)) == 1:
                flag += 1
                add_letter(dic, word, word_dic.get(word)[0])
            if len(word_dic.get(word)) == 0:
                return False
        if flag > flag_pre:
            flag_pre = flag
        else:
            break
    return dic


def longest_word(dic: dict):
#寻找最长单词，优先以长的单词构造映射
    word_longest = ''
    length = 0
    word_length = 0
    for word in dic:
        if len(word) >= length and len(dic[word]) != 0:
            if len(word) > length:
                word_longest = word
                length = len(word)
                word_length = len(dic[word])
            else:
                if len(dic[word]) < word_length:
                    word_longest = word.copy()
                    length = len(word)
                    word_length = len(dic[word])
    return word_longest, word_length


def attack(text: str, dictionary: list):
    order, word_list = initial(text) #字频，原文单词表
    print(order)
    dic = {}
    word_dic = analyse(dictionary, word_list, 'e', order[0]) #{包含该字母(e)的单词:与之对应的能产生单射的单词列表}
    flag_pre = 0
    flag = 0
    for word in word_dic: #优先拿单词对唯一的
        if len(word_dic.get(word)) == 1:
            flag_pre += 1
            add_letter(dic, word, word_dic.get(word)[0])
    if flag_pre == 0:
        word_longest, l = longest_word(word_dic)
        for i in range(l):
            if analyse_dic(dic, word_dic, word_longest):
                dic = analyse_dic(dic, word_dic, word_longest) #贪心，只要能产生映射就选它了
                break
            else:
                word_dic[word].remove(word_dic[word][0]) #否则就不停的删直到该单词只剩一个单词能与之对应

    while not is_finish(word_dic):
		#首先清除那些无法满足先前映射的单词 贪心
        for word in word_dic:
            word_check_list = word_dic.get(word)
            if len(word_check_list) != 1:
                for word_check in word_check_list:
                    if not check_letter(dic, word, word_check):
                        word_check_list.remove(word_check)
        flag = 0
		#先添加对应单词唯一的
        for word in word_dic:
            if len(word_dic.get(word)) == 1:
                flag += 1
                add_letter(dic, word, word_dic.get(word)[0])
        if flag > flag_pre:
            flag_pre = flag#如果映射表完成了

        else:
			#再优先选取长的
            word_longest, l = longest_word(word_dic)
            for i in range(l):
                if analyse_dic(dic, word_dic, word_longest):
                    dic = analyse_dic(dic, word_dic, word_longest)
                    break
                else:
                    word_dic[word].remove(word_dic[word][0])
            break
    print("after_e", word_dic)
    order, word_list = initial(text)
	
	#一下相同，对第二频率的字母进行处理
    word_dic = analyse(dictionary, word_list, 't', order[1])
    flag_pre = 0
    for word in word_dic:
        if len(word_dic.get(word)) == 1:
            flag_pre += 1
            add_letter(dic, word, word_dic.get(word)[0])
    if flag_pre == 0:
        word_longest, l = longest_word(dic_checked)
        for i in range(l):
            if analyse_dic(dic, word_dic, word_longest):
                break
            else:
                word_dic[word].remove(word_dic[word][0])
    while not is_finish(word_dic):
        for word in word_dic:
            word_check_list = word_dic.get(word)
            if len(word_check_list) != 1:
                for word_check in word_check_list:
                    if not check_letter(dic, word, word_check):
                        word_check_list.remove(word_check)
        for word in word_dic:
            if len(word_dic.get(word)) == 1:
                add_letter(dic, word, word_dic.get(word)[0])
        flag = 0
        for word in word_dic:
            if len(word_dic.get(word)) == 1:
                flag += 1
                add_letter(dic, word, word_dic.get(word)[0])
        if flag > flag_pre:
            flag_pre = flag
        else:
            break
    print("after_t", word_dic)
    return dic


def decode(text: str, dic: dict):
	#依据映射表decode
    result = ''
    for c in text:
        if c in dic:
            result += dic[c]
        else:
            result += c
    return result


def correct_rate(text: str):
	#正确率（正确单词个数/总个数）
    d = enchant.Dict("en_US")
    word_list = text.split()
    l = len(word_list)
    num = 0
    for word in word_list:
        if d.check(word):
            num += 1
    return num / l


def boom(text: str, dic: dict):
	#剩余字母爆破
    possi_result = []
    rate = []
    other_key = []
    other_value = []
    key, value = list(dic.keys()), list(dic.values())
    for i in range(26):
        c = letter(i)
        if c not in key:
            other_key.append(c)
        if c not in value:
            other_value.append(c)
    pailie = list(itertools.permutations(other_value))
    for x in pailie:
        x = list(x)
        l = len(x)
        for i in range(l):
            dic[other_key[i]] = x[i]
        s = decode(text, dic)
        if s not in possi_result:
            possi_result.append(s)
            rate.append(correct_rate(s))
    return possi_result, rate


if __name__ == '__main__':
    text = '''bg kswgsuwqo sisgcge, ks ksgj jb jds jbkg dqvv. cj kqu jds vquj wqo bx jds osqn qgw q vqnes znbkw bx ysbyvs dqw eqjdsnsw mgwsn jds jbkg dqvv zvbzf. cj kbmvw ujncfs jksvis cg jksgjo tcgmjsu' jcts. xcxjssg tcgmjsu yquusw qgw jdsg, qj xcis jb jksvis, jds zvbzf  ujbyysw. jds ace tcgmjs dqgw wcw gbj tbis. ks kqcjsw qgw kqcjsw, amj gbjdcge dqyysgsw. umwwsgvo ubtsbgs udbmjsw, ‘cj'u jkb tcgmjsu yquj jksvis! jds zvbzf  dqu ujbyysw!' c vbbfsw qj to kqjzd. cj kqu jnms. jds ace zvbzf nsxmusw jb ksvzbts jds gsk osqn. qj jdqj tbtsgj, sisnoabwo aseqg jb vqmed qgw ucge.'''
    # text = '''gf vtrftlrqn tctfofu, vt vtfz zg zit zgvf iqss. oz vql zit sqlz rqn gy zit ntqk qfr q sqkut ekgvr gy htghst iqr uqzitktr xfrtk zit zgvf iqss esgea. oz vgxsr lzkoat zvtsct of zvtfzn dofxztl' zodt. yoyzttf dofxztl hqlltr qfr zitf, qz yoct zg zvtsct, zit esgea  lzghhtr. zit wou dofxzt iqfr ror fgz dgct. vt vqoztr qfr vqoztr, wxz fgziofu iqhhtftr. lxrrtfsn lgdtgft ligxztr, ‘oz'l zvg dofxztl hqlz zvtsct! zit esgea  iql lzghhtr!" o sggatr qz dn vqzei. oz vql zkxt. zit wou esgea ktyxltr zg vtsegdt zit ftv ntqk. qz ziqz dgdtfz, tctknwgrn wtuqf zg sqxui qfr lofu.'''
    dictionary = open('words.txt', 'r').read().lower().splitlines()
    # test="On Wednesday evening, we went to the Town Hall. It was the last day of the year and a large crowd of people had gathered under the Town Hall clock. It would strike twelve in twenty minutes' time. Fifteen minutes passed and then, at five to twelve, the clock  stopped. The big minute hand did not move. We waited and waited, but nothing happened. Suddenly someone shouted, ‘It's two minutes past twelve! The clock  has stopped!' I looked at my watch. It was true. The big clock refused to welcome the New Year. At that moment, everybody began to laugh and sing."
    # for each in "!\"#$%&()*+,./:;<=>?@[\\]^_{}|~-'’‘":
    #     test = test.replace(each, " ")
    # test=test.lower().split()
    # for word in test:
    #     if word not in dictionary:
    #         print(word)
    dic = attack(text[:], dictionary)
    possi_result, rate = boom(text, dic)
    for i in range(10):
        if i == len(possi_result):
            break
        rate_max = max(rate)
        index = rate.index(rate_max)
        rate[index] = 0
        print(possi_result[index])
        print("correct rate=", rate_max)
'''
"On Wednesday evening, we went to the Town Hall. It was the last day of the year and a large crowd of people had gathered under the Town Hall clock. It would strike twelve in twenty minutes' time. Fifteen minutes passed and then, at five to twelve, the clock  stopped. The big minute hand did not move. We waited and waited, but nothing happened. Suddenly someone shouted, ‘It's two minutes past twelve! The clock  has stopped!' I looked at my watch. It was true. The big clock refused to welcome the New Year. At that moment, everybody began to laugh and sing."

'''
