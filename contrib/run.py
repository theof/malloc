# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run.py                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvallee <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/15 15:28:14 by tvallee           #+#    #+#              #
#    Updated: 2017/12/15 15:49:50 by tvallee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import random

LAST_CRASHED = FALSE

def render_template(variables):
    filename = 'use_malloc.c'
    with open(filename + '.template', 'r') as src:
        with open(filename, 'w') as dst:
            dst.write(src.read().format(**variables()))

def evaluate(array):
    pass

def clean():
    pass

def make_random_instruction(array):
    malloced = [instruction for instruction in array if instruction ]

def begin():
    array = []
    while not LAST_CRASHED:
        array.append(make_random_instruction(array))
        LAST_CRASHED = evaluate(array)

def clean():
    pass

if __name__ == '__main__':
    try:
        begin()
    except KeyboardInterrupt:
        clean()
