import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Scanner;

//Этот класс будет просто возращать строчки из консоли или из файла
public class Reading {
    private int type; //0 - консоль, 1 - файл
    private Path pathFile;
    private BufferedReader buffer;
    private int checkPath(Path path){ //Вернём 1 если всё ок, 0 если всё плохо
        if ((Files.exists(path) == true) && (Files.isReadable(path) == true)){
            return 1;
        }
        else{
            return 0;
        }
    }
    public Reading(String[] args){
        if(args.length == 0){
            type = 0;
        }
        else {
            Path path = Paths.get(args[0]);
            if (checkPath(path) == 0){
                path = Paths.get(System.getProperty("user.dir") + args[0]);
                if (checkPath(path) == 0){
                    System.out.println("Error in open file! Check name file, or directory.");
                    System.out.println("Switching to console mode:");
                    type = 0;
                    return;
                }
            }
            pathFile = path;
            try {
                buffer = Files.newBufferedReader(path, StandardCharsets.UTF_8);
                System.out.println("File opened! Switching to file mode:");
                type = 1;
            }
            catch (IOException e) {
                System.err.println("Error in read file! " + e.getMessage());
                type = 0;
            }
        }
    }
    public String nextLine(){
        if (type == 0){ //консолька
            Scanner console = new Scanner(System.in);
            return console.nextLine();
        }
        else if (type == 1){ //файл
            try{
                String str;
                if ((str = buffer.readLine()) != null){
                    return str;
                }
                else{
                    return "";
                }
            }
            catch (IOException e){
                System.err.println("Error in reading file! " + e.getMessage());
                return null;
            }
        }
        else{
            System.err.println("Unknown mode of reading! ");
            return null;
        }
    }
    public String getPathFile(){
        return pathFile.toString();
    }
}
