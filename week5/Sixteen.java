import javax.lang.model.type.NullType;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Sixteen {

    public static String read(String filepath) {
        // a simple check on stack trace
        String name = Thread.currentThread().getStackTrace()[2].getMethodName();
        if(name!="invoke0"&&name!="lambda$reduce$1") return null;

        try(Stream<String> lines = Files.lines(Paths.get(filepath))) {
            return lines.collect(Collectors.joining(System.lineSeparator()));
        } catch (IOException ioe) {
            throw new RuntimeException(ioe);
        }
    }

    public static String filter(String data) {
        return data.replaceAll("[^a-zA-Z\\s\\t\\d]", " ").toLowerCase();
    }

    public static List<String> scan(String data) {
        return Arrays.asList(data.split("\\s+"));
    }

    public static Function<String, List<String>> reduce(List<String> words) {
        return stopWordsFilepath -> {
            Set<String> stopWords = Arrays.stream(read(stopWordsFilepath).split(",")).collect(Collectors.toCollection(HashSet::new));
            return words.stream().filter(w -> !stopWords.contains(w) && w.length() > 1).collect(Collectors.toList());
        };
    }

    public static Map<String, Integer> freq(List<String> words) {
        final Map<String, Integer> wordFreqs = new HashMap<>();
        words.forEach(word -> wordFreqs.merge(word, 1, (oldVal, newVal) -> oldVal + newVal));
        return wordFreqs;
    }

    public static List<Pair<String, Integer>> sort(Map<String, Integer> wordFreqs) {
        return wordFreqs.entrySet().stream().sorted((e1, e2) -> -e1.getValue().compareTo(e2.getValue())).
                map(entry -> new Pair<>(entry.getKey(), entry.getValue())).collect(Collectors.toList());
    }

    public static void pr(List<Pair<String, Integer>> wordFreqs) {
        if (wordFreqs.size() > 0) {
            Pair<String, Integer> head = wordFreqs.get(0);
            System.out.println(String.format("%s  -  %d", head.mItem1, head.mItem2));
            pr(wordFreqs.size() > 1 ? wordFreqs.subList(1, wordFreqs.size()) : new ArrayList<>());
        }
    }

    private static class Pair<T1, T2> {
        private final T1 mItem1;
        private final T2 mItem2;
        Pair(T1 item1, T2 item2) {
            mItem1 = item1;
            mItem2 = item2;
        }
    }

    public static void main(String[] args) throws Exception{
        Class<?> c = Class.forName("Sixteen");
        String str = (String)c.getMethod("read", java.lang.String.class).invoke(null, args[0]);
        str = (String)c.getMethod("filter", java.lang.String.class).invoke(null, str);
        List<String> stringList = (List<String>)c.getMethod("scan", java.lang.String.class).invoke(null, str);
        Function<String, List<String>> stringListFunction = (Function<String, List<String>>)c.getMethod("reduce", java.util.List.class).invoke(null, stringList);
        Map<String, Integer> stringIntegerMap = (Map<String, Integer>)c.getMethod("freq", java.util.List.class).invoke(null, stringListFunction.apply(args[1]));
        List<Pair<String, Integer>> pairList = (List<Pair<String, Integer>>)c.getMethod("sort", java.util.Map.class).invoke(null, stringIntegerMap);
        Object answer = c.getMethod("pr", java.util.List.class).invoke(null, pairList.subList(0, 25));
    }
}
