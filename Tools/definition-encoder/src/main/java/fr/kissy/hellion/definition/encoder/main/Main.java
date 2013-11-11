package fr.kissy.hellion.definition.encoder.main;

import fr.kissy.hellion.definition.encoder.main.parser.ApplicationParser;
import fr.kissy.hellion.definition.encoder.main.parser.SceneParser;

import javax.management.modelmbean.XMLParseException;
import java.io.File;
import java.io.IOException;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: Main.java 32 2012-03-29 14:44:19Z kissy $
 */
public class Main {
    
    public static void main(String[] args) throws IOException, XMLParseException {
        if (args.length < 2) {
            System.out.println("The first argument must be the file to process, " +
                    "and the second argument must be the output path.");
            return;
        }
        
        // Test if adf file exists and find the directory.
        String applicationPath = args[0];
        File applicationFile = new File(applicationPath);
        if (!applicationFile.exists() || !applicationFile.canRead()) {
            System.out.println("File " + applicationFile.getAbsoluteFile() + " not found.");
            return;
        }

        // Test if the output path exists
        String outputPath = args[1];
        File outputFile = new File(outputPath);
        if (!outputFile.exists() || !outputFile.canWrite()) {
            System.out.println("Output directory " + outputFile + " cannot be written.");
            return;
        }

        // Process the file
        generateFile(applicationFile, outputFile);
    }

    /**
     * Convert the ADF File into a binary file.
     *
     * @param rawGdcFile The adf file as the user gave it.
     * @param outputFile The output file as the user gave it.
     * @throws IOException Exception.
     * @throws XMLParseException Exception.
     */
    private static void generateFile(File rawGdcFile, File outputFile) throws IOException, XMLParseException {
        File applicationFile = rawGdcFile.getAbsoluteFile();
        File applicationDirectory = new File(applicationFile.getParent());

        ApplicationParser applicationParser = new ApplicationParser(applicationFile.getAbsolutePath(), outputFile.getAbsolutePath());
        applicationParser.writeBuilder();

        for (String scene : applicationParser.getApplicationBuilder().getScenesList()) {
            File sceneFile = new File(applicationDirectory.getAbsolutePath() + "/" + scene + ".sdf");
            if (!sceneFile.exists() || !sceneFile.canRead()) {
                System.out.println("File " + sceneFile.getAbsoluteFile() + " not found.");
                return;
            }

            SceneParser sceneParser = new SceneParser(sceneFile.getAbsolutePath(), outputFile.getAbsolutePath(), applicationParser.getSystems());
            sceneParser.writeBuilder();
        }
    }

}
