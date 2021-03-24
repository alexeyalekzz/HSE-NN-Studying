// Copyright (c) 2021 Alexey Klimov
//
// Created by Alexey on 19.03.2021.
//

#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#define STONE_SYM '#'
#define PREY_SYM 'f'
#define PREDATOR_SYM 'S'

#define PREY_LIFETIME 7
#define PREDATOR_LIFETIME 15
#define PREDATOR_NO_FOOD_MAX 4

#define PREY_REPR_TIME 2
#define PREDATOR_REPR_TIME 7


#define OBJECTS_N 3
enum class OBJECTS : int { STONE, PREY, PREDATOR };

#endif  // INCLUDE_COMMON_H_
