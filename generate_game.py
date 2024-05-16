from ctypes import ArgumentError
import copy
import random
from tqdm import tqdm
import json

def filter_in_letters(strs, letters, with_last_letter=False):
    letters = letters.upper()
    filtered = []
    for s in strs:
        if all(map(lambda c: c in letters, s)):
            if not with_last_letter or letters[-1] in s:
                filtered.append(s)
    return filtered

def check(setka, letter, x, y, mode):
    if setka[y][x] != '*' and setka[y][x] != letter:
        return False
    if x >= 1 and y >= 1 and \
          setka[y][x - 1] != '*' and \
          setka[y - 1][x] != '*' and \
          setka[y - 1][x - 1] != '*':
        return False
    if x >= 1 and y < len(setka) - 1 and \
          setka[y][x - 1] != '*' and \
          setka[y + 1][x] != '*' and \
          setka[y + 1][x - 1] != '*':
        return False
    if x < len(setka[0]) - 1 and y >= 1 and \
          setka[y][x + 1] != '*' and \
          setka[y - 1][x] != '*' and \
          setka[y - 1][x + 1] != '*':
        return False
    if x < len(setka[0]) - 1 and y < len(setka) - 1 and \
          setka[y + 1][x] != '*' and \
          setka[y][x + 1] != '*' and \
          setka[y + 1][x + 1] != '*':
        return False
    if setka[y][x] == '*':
        if mode == 'horizontal':
            if y >= 1 and setka[y - 1][x] != '*':
                return False
            if y < len(setka) - 1 and setka[y + 1][x] != '*':
                return False
        else:
            if x >= 1 and setka[y][x - 1] != '*':
                return False
            if x < len(setka[0]) - 1 and setka[y][x + 1] != '*':
                return False
    return True

def place_word_on_position(word, setka, x, y, mode):
    setka = copy.deepcopy(setka)
    if mode != 'horizontal' and mode != 'vertical':
        raise ArgumentError('param mode can be only horizontal or vertical')
    if mode == 'horizontal':
        if x >= 1 and setka[y][x - 1] != '*':
            return False, setka
        if x < len(setka[0]) - 1 and setka[y][x + 1] != '*':
            return False, setka
    else:
        if y >= 1 and setka[y - 1][x] != '*':
            return False, setka
        if y < len(setka) - 1 and setka[y + 1][x] != '*':
            return False, setka

    for i in range(len(word)):
        if not check(setka, word[i], x, y, mode):
            return False, setka
        setka[y][x] = word[i]
        if mode == 'horizontal':
            x += 1
        else:
            y += 1
    if y < len(setka) and x < len(setka[0]) and setka[y][x] != '*':
        return False, setka
    return True, setka

def print_setka(setka):
    for row in setka:
        print('   '.join(row))
        print()

def dfs(graph, visited, i=0):
    visited[i] = True
    for j in graph[i]:
        if not visited[j]:
            dfs(graph, visited, j)

def is_graph_fully_connected(graph):
    visited = [False] * len(graph)
    dfs(graph, visited)
    return all(visited)
    
def get_normalized_words_pos(words_pos):
    norm_words_pos = {}
    min_x = 100000000
    min_y = 100000000
    for word, (x, y, mode) in words_pos.items():
        if x < min_x:
            min_x = x
        if y < min_y:
            min_y = y
    for word, (x, y, mode) in words_pos.items():
        norm_words_pos[word] = (x-min_x, y-min_y, mode)
    return norm_words_pos

def setka_is_correct(setka, words_pos, min_cross_count, all_good_pos):
    for word, pos in words_pos.items():
        x, y, mode = pos
        cross_count = 0
        for _ in range(len(word)):
            if mode == 'horizontal':
                if y >= 1 and setka[y - 1][x] != '*' or y < len(setka) - 1 and setka[y + 1][x] != '*':
                    cross_count += 1
                x += 1
            else:
                if x >= 1 and setka[y][x - 1] != '*' or x < len(setka[0]) - 1 and setka[y][x + 1] != '*':
                    cross_count += 1
                y += 1
        if cross_count < min_cross_count:
            return False
    setka_words = [[[] for _ in range(len(setka[0]))] for _ in range(len(setka))]
    graph = [[] for _ in range(len(words_pos))]
    for i, (word, (x, y, mode)) in enumerate(words_pos.items()):
        for _ in range(len(word)):
            for word_index in setka_words[y][x]:
                if i in graph[word_index]:
                    return False
                graph[word_index].append(i)
                graph[i].append(word_index)
            setka_words[y][x].append(i)
            if mode == 'horizontal':
                x += 1
            else:
                y += 1
    if not is_graph_fully_connected(graph):
        return False
    normalized_pos = get_normalized_words_pos(words_pos)
    if normalized_pos in all_good_pos:
        return False
    all_good_pos.append(normalized_pos)
    return True


def get_all_ways(graph, first_word):
    visited = {word: False for word in graph}
    temp_way = {}
    all_ways = []
    dfs_get_ways(graph, first_word, all_ways, visited, temp_way)
    return all_ways


def dfs_get_ways(graph, temp_word, all_ways, visited, temp_way, parent_word=None):
    visited = visited.copy()
    temp_way = temp_way.copy()
    if parent_word is not None:
        if parent_word not in temp_way:
            temp_way[parent_word] = []
        temp_way[parent_word].append(temp_word)
    visited[temp_word] = True

    if all(visited.values()):
        all_ways.append(temp_way)
        return

    for word_connected in graph[temp_word]:
        if not visited[word_connected]:
            dfs_get_ways(graph, word_connected, all_ways, visited, temp_way, temp_word)


def get_graph(words):
    graph = {word: {} for word in words}
    for i in range(len(words)):
        for j in range(i, len(words)):
            for i1, c1 in enumerate(words[i]):
                for i2, c2 in enumerate(words[j]):
                    if c1 == c2:
                        if words[j] not in graph[words[i]]:
                            graph[words[i]][words[j]] = []
                        if words[i] not in graph[words[j]]:
                            graph[words[j]][words[i]] = []
                        graph[words[i]][words[j]].append((i1, i2))
                        graph[words[j]][words[i]].append((i2, i1))
    return graph


def place_words(words, min_cross_count=1, return_all=False):
    graph = get_graph(words)
    first_word = max(graph.keys(), key=len)
    all_graph_ways = get_all_ways(graph, first_word)
    if len(all_graph_ways) == 0:
        return
    all_good_pos = []
    for way in all_graph_ways:
        if not place_words_by_way(graph, way, min_cross_count, all_good_pos=all_good_pos, return_all=return_all):
            break
    return all_good_pos


def place_words_by_way(graph, way, min_cross_count=1, parent=None, placed=None, setka=None, words_pos=None, all_good_pos=None, return_all=False):
    if setka is None:
        setka = [['*'] * 8 for _ in range(6)]
    if placed is None:
        placed = {word: False for word in graph}
    if words_pos is None:
        words_pos = {}
    if all_good_pos is None:
        all_good_pos = []
    if all(placed.values()):
        if not setka_is_correct(setka, words_pos, min_cross_count, all_good_pos):
            return True
        if not return_all:
            print(words_pos)
            print_setka(setka)
            if input('Enter exit to exit: ').lower() == 'exit':
                return False
        return True
    
    if parent is None:
        first_word = max(graph.keys(), key=len)
        placed[first_word] = True
        for i in range(0, len(setka) - len(first_word) + 1):
            for j in range(0, len(setka[0])):
                _, new_setka = place_word_on_position(first_word, setka, j, i, mode='vertical')
                new_words_pos = words_pos.copy()
                new_words_pos[first_word] = (j, i, 'vertical')
                if not place_words_by_way(graph, way, min_cross_count, first_word, placed, new_setka, new_words_pos, all_good_pos, return_all=return_all):
                    return False
                
        for i in range(0, len(setka)):
            for j in range(0, len(setka[0]) - len(first_word) + 1):
                _, new_setka = place_word_on_position(first_word, setka, j, i, mode='horizontal')
                new_words_pos = words_pos.copy()
                new_words_pos[first_word] = (j, i, 'horizontal')
                if not place_words_by_way(graph, way, min_cross_count, first_word, placed, new_setka, new_words_pos, all_good_pos, return_all=return_all):
                    return False
    else:
        p_x, p_y, p_mode = words_pos[parent]
        for word in way[parent]:
            if placed[word]:
                continue
            for i1, i2 in graph[parent][word]:
                if p_mode == 'horizontal':
                    mode = 'vertical'
                    x = p_x + i1
                    y = p_y - i2
                else:
                    mode = 'horizontal'
                    x = p_x - i2
                    y = p_y + i1
                if x < 0 or y < 0 or x >= len(setka[0]) or y >= len(setka):
                    continue
                if mode == 'horizontal' and x + len(word) >= len(setka[0]) or \
                            mode == 'vertical' and y + len(word) >= len(setka):
                    continue
                is_correct, new_setka = place_word_on_position(word, setka, x, y, mode=mode)
                if is_correct:
                    new_words_pos = words_pos.copy()
                    new_words_pos[word] = (x, y, mode)
                    new_placed = placed.copy()
                    new_placed[word] = True
                    if not place_words_by_way(graph, way, min_cross_count, word, new_placed, new_setka, new_words_pos, all_good_pos, return_all=return_all):
                        return False
    return True

def generate_any_words_with_min_cross_count(chosen_words, min_cross_count, min_words_count=6):
    chosen_words = chosen_words.copy()
    print('Search for:', len(chosen_words))
    generated = place_words(chosen_words, min_cross_count=min_cross_count)
    while generated is None or len(generated) == 0:
        word_for_remove = random.choice(chosen_words)
        chosen_words.remove(word_for_remove)
        if len(chosen_words) < min_words_count:
            return None
        print('Search for:', len(chosen_words))
        generated = place_words(chosen_words, min_cross_count=min_cross_count)
    return generated


def choose_next_word(chosen_words, graph, previous_word):
    max_len = 0
    max_new_word = None
    for new_word, value in graph[previous_word].items():
        if new_word not in chosen_words and len(value) > max_len:
            max_new_word = new_word
            max_len = len(value)
    return max_new_word

def choose_and_place_words(words, max_words_on_setka=10, min_words_count=3):
    if len(words) > max_words_on_setka:
        temp_word = random.choice(words)
        chosen_words = []
        chosen_words.append(temp_word)
        graph = get_graph(words)
        for _ in range(max_words_on_setka-1):
            temp_word = choose_next_word(chosen_words, graph, temp_word)
            chosen_words.append(temp_word)
        print(chosen_words)
    else:
        chosen_words = words

    generated = generate_any_words_with_min_cross_count(chosen_words, 2, min_words_count)
    if generated is None or len(generated) == 0:
        generated = generate_any_words_with_min_cross_count(chosen_words, 1, min_words_count)
    return generated

def get_actual_letters(level):
    words_count = len(list(level.keys()))
    letters = {}
    for word in level:
        temp_letters = {}
        for c in word:
            if c not in temp_letters:
                temp_letters[c] = 1
            else:
                temp_letters[c] += 1
        for c, count in temp_letters.items():
            if c not in letters or count > letters[c]:
                letters[c] = count
    actual_letters = []
    for c, count in letters.items():
        actual_letters += [c] * count
    return actual_letters
    
def place_new_word_on_setka(graph, new_word, setka, words_pos, all_good_pos, max_letters):
    new_words_pos = words_pos.copy()
    new_words_pos[new_word] = None
    letters_count = len(get_actual_letters(new_words_pos))
    if letters_count >= max_letters:
        return
    for placed_word in words_pos:
        if new_word not in graph[placed_word]:
            continue
        p_x, p_y, p_mode = words_pos[placed_word]
        for i1, i2 in graph[placed_word][new_word]:
            if p_mode == 'horizontal':
                mode = 'vertical'
                x = p_x + i1
                y = p_y - i2
            else:
                mode = 'horizontal'
                x = p_x - i2
                y = p_y + i1
            if x < 0 or y < 0 or x >= len(setka[0]) or y >= len(setka):
                continue
            if mode == 'horizontal' and x + len(new_word) >= len(setka[0]) or \
                        mode == 'vertical' and y + len(new_word) >= len(setka):
                continue
            is_correct, new_setka = place_word_on_position(new_word, setka, x, y, mode=mode)
            if is_correct:
                new_words_pos[new_word] = (x, y, mode)
                setka_is_correct(new_setka, new_words_pos, 1, all_good_pos)

def get_setka_from_words_pos(words_pos):
    setka = [['*'] * 8 for _ in range(6)]
    for word, (x, y, mode) in words_pos.items():
        for c in word:
            setka[y][x] = c
            if mode == 'horizontal':
                x += 1
            else:
                y += 1
    return setka

def get_count_from_words_pos(words_pos):
    setka = [[0] * 8 for _ in range(6)]
    count = 0
    for word, (x, y, mode) in words_pos.items():
        for _ in word:
            setka[y][x] += 1
            if setka[y][x] > 1:
                count += 1
            if mode == 'horizontal':
                x += 1
            else:
                y += 1
    return count

def generate_from_words(words, min_words_count, max_letters):
    if words is None or len(words)==0:
        return None
    if len(words) > 24:
        words = random.choices(words, k=24)
    graph = get_graph(words)
    if len(words) > 1:
        temp_word = random.choice(words)
        chosen_words = []
        chosen_words.append(temp_word)
        for _ in range(1-1):
            temp_word = choose_next_word(chosen_words, graph, temp_word)
            chosen_words.append(temp_word)
    else:
        chosen_words = words

    generated = place_words(chosen_words, min_cross_count=0, return_all=True)

    final_words_pos = []
    while True:
        new_generated = []
        for words_pos in generated:
            free_words = list(filter(lambda w: w not in words_pos, words))
            temp_setka = get_setka_from_words_pos(words_pos)
            before = len(new_generated)
            for new_word in free_words:
                place_new_word_on_setka(graph, new_word, temp_setka, words_pos, new_generated, max_letters)
            after = len(new_generated)
            if after - before == 0 and len(list(words_pos.keys())) >= min_words_count and all(map(lambda wp: set(wp.keys()) != set(words_pos.keys()), final_words_pos)):
                final_words_pos.append(words_pos)
        if len(new_generated) == 0:
            break
        else:
            generated = new_generated
    return sorted(final_words_pos, key=get_count_from_words_pos, reverse=True)

def generate_levels_from_words(words, min_words_count, max_letters, num_levels):
    levels = []
    words = words.copy()
    while(len(levels) < num_levels):
        new_gen = generate_from_words(words, min_words_count, max_letters)
        if new_gen is None or len(new_gen) == 0:
            break
        new_level = new_gen[0]
        levels.append(new_level)
        for word in new_level:
            words.remove(word)
    return levels
    
def factorial(num):
    mult = 1
    for i in range(1, num + 1):
        mult *= i
    return mult

def generate_whole_game(all_letters, strs):
    all_levels = {}
    filtered = filter_in_letters(strs, all_letters[:6], with_last_letter=False)
    first_level = generate_levels_from_words(filtered, 4, 6, 3)
    s = ''
    for index, level in enumerate(first_level, 1):
        words_count = len(list(level.keys()))
        actual_letters = get_actual_letters(level)
        letters_count = len(actual_letters)
        s += f'{index}\t{words_count}\t{letters_count}\t'
        s += '\t'.join(actual_letters)
        for word, (x, y, mode) in level.items():
            s += f'\r{len(word)}\t{word}\t{x}\t{y}\t{mode}'
        if index != len(first_level):
            s += '\n\r\n'
    with open("/Users/andreyvesnin/Desktop/game_levels/stage_1.mch", "w") as file:
        file.write(s)
    with open("/Users/andreyvesnin/Desktop/game_levels/first.mch", "w") as json_file:
        json.dump(first_level, json_file)
    all_levels['first'] = first_level
    for i in tqdm(range(7, len(all_letters))):
        num_iterations = min(factorial(i-1) // factorial(i-6) // factorial(5), 25)
        new_levels = []
        for k in range(4, 7):
            for _ in range(num_iterations):
                temp_letters = ''.join(random.choices(all_letters[:i-1], k=k))
                temp_letters += all_letters[i-1]
                filtered = filter_in_letters(strs, temp_letters, with_last_letter=False)
                for level in new_levels:
                    for word in level:
                        if word in filtered:
                            filtered.remove(word)
                temp_generated = generate_levels_from_words(filtered, k, k+2, k)
                for level in temp_generated:
                    if level not in new_levels:
                        new_levels.append(level)
        s = ''
        for index, level in enumerate(new_levels, 1):
            words_count = len(list(level.keys()))
            actual_letters = get_actual_letters(level)
            letters_count = len(actual_letters)
            s += f'{index}\t{words_count}\t{letters_count}\t'
            s += '\t'.join(actual_letters)
            for word, (x, y, mode) in level.items():
                s += f'\r{len(word)}\t{word}\t{x}\t{y}\t{mode}'
            if index != len(new_levels):
                s += '\n\r\n'
        with open(f"game_levels/stage_{i-5}.mch", "w") as file:
            file.write(s)
#        with open(f"/Users/andreyvesnin/Desktop/game_levels/{all_letters[i - 1]}.mch", "w") as json_file:
#            json.dump(new_levels, json_file)
        all_levels[all_letters[i - 1]] = new_levels
    return all_levels

if __name__ == "__main__":
    with open("file.csv", encoding='1251') as file:
        strs = []
        for s in file:
            params = s.strip().split(';')
            if int(params[1]) <= 2 and len(params[7]) <= 7 and len(params[7]) > 2:
                strs.append(params[7])
                
    generate_whole_game('оеанитфсрвлэщцукмджюшпяыьгзбчйх', strs)
