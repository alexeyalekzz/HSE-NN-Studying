package com.company;

import java.time.format.DateTimeParseException;
import java.util.Scanner;
import java.time.LocalDate;


public class Main {

    public static void main(String[] args) {
        var scanner = new Scanner(System.in);

        System.out.println("Введите ФИО");
        String fullname = scanner.nextLine();

        System.out.println("Введите дату рождения (e.g. 2007-12-21)");
        String date = scanner.nextLine();

        printData(fullname, date);
    }

    private static void printData(String fullname, String date) {
        LocalDate dateOfBirth;

        // check date string
        try {
            dateOfBirth = LocalDate.parse(date);
        } catch (DateTimeParseException e) {
            System.out.println("Неверный формат даты!");
            return;
        }

        // check name
        String[] splitted = fullname.split(" ");
        if (splitted.length != 3) {
            System.out.println("Введено не полное ФИО!\n");
            return;
        }

        // check date
        int age = LocalDate.now().compareTo(dateOfBirth);
        if (age < 0) {
            System.out.println("Дата рождения не может быть в будущем!");
            return;
        }

        // write name
        System.out.printf("Имя: %s %c. %c.\n",
                splitted[0],
                splitted[1].charAt(0),
                splitted[2].charAt(0));

        // check gender
        Gender gender = getGenderByName(fullname);
        if (gender != Gender.Unknown) {
            // write gender
            System.out.println("Пол: " + gender.name());
        }

        // write age
        System.out.print("Возраст: " + age);
        if (age % 10 == 1) {
            System.out.println(" год");
        } else if (age % 100 > 10 && age % 100 < 20 || age % 10 == 0) {
            System.out.println(" лет");
        } else if (age % 10 == 2 || age % 10 == 3 || age % 10 == 4) {
            System.out.println(" года");
        }
    }

    private static Gender getGenderByName(String name) {
        String[] maleEnding = new String[]{"ов", "ев" ,"ин" ,"ын", "ой", "цкий", "ский", "цкой", "ской"};
        String[] femaleEnding = new String[]{"ова", "ева", "ина", "ая", "яя", "екая", "цкая"};

        for (String ending : maleEnding) {
            if (name.contains(ending)) {
                return Gender.Male;
            }
        }

        for (String ending : femaleEnding) {
            if (name.contains(ending)) {
                return Gender.Female;
            }
        }

        return Gender.Unknown;
    }
}

enum Gender { Unknown, Male, Female }
