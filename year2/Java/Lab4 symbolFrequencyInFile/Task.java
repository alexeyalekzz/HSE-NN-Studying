package task;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.Scanner;

public class Task {
    public static void printFrequency(String fin, String fout) {
        String text = readFile(fin);
        HashMap<Character, Integer> result = countSymbols(text);
        printResult(result, fout);
    }

    private static void printResult(HashMap<Character, Integer> res, String filename) {
        try (var fileWriter = new FileWriter(filename, false)) {
            fileWriter.write("Character:  |  Frequency:\n\n");

            var sorted = sortHashMap(res);
            for (var c : sorted.keySet()) {
                fileWriter.write("\t'" + c.toString() + "' \t\t\t" + sorted.get(c).toString() + "\n");
            }

        } catch (IOException e) {
            System.out.println("An error occurred with output file");
            e.printStackTrace();
        }
    }

    private static HashMap<Character, Integer> sortHashMap(HashMap<Character, Integer> map) {
        var list = new LinkedList<>(map.entrySet());
        list.sort((a, b) -> b.getValue().compareTo(a.getValue()));

        var sorted = new LinkedHashMap<Character, Integer>();
        for (var en : list) {
            sorted.put(en.getKey(), en.getValue());
        }
        return sorted;
    }

    private static HashMap<Character, Integer> countSymbols(String text) {
        var result = new HashMap<Character, Integer>();

        for (int i = 0; i < text.length(); i++) {
            var c = text.charAt(i);
            if (c != ' ' && c != '\t') {
                var f = result.get(c);
                result.put(c, f == null ? 1 : f + 1);
            }
        }

        return result;
    }

    private static String readFile(String file) {
        StringBuilder result = new StringBuilder();

        try (var fileReader = new Scanner(new FileReader(file))) {
            while (fileReader.hasNextLine()) {
                result.append(fileReader.nextLine()).append(' ');
            }
            return result.toString();

        } catch (FileNotFoundException e) {
            System.out.println("Input file not found!");
            e.printStackTrace();
            return "";
        }
    }
}
