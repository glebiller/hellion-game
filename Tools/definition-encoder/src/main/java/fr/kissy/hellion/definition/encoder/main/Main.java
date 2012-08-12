package fr.kissy.hellion.definition.encoder.main;

import fr.kissy.hellion.definition.encoder.main.parser.GlobalDefinitionParser;
import fr.kissy.hellion.definition.encoder.main.parser.SceneDefinitionParser;
import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;
import fr.kissy.hellion.definition.encoder.proto.GlobalDefinitionDto;

import javax.management.modelmbean.XMLParseException;
import java.io.File;
import java.io.FileInputStream;
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
        
        // Test if gdf file exists and find the directory.
        String gdfPath = args[0];
        File globalDefinitionFile = new File(gdfPath);
        if (!globalDefinitionFile.exists() || !globalDefinitionFile.canRead()) {
            System.out.println("File " + gdfPath + " not found.");
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
        generateFile(globalDefinitionFile, outputFile);
    }

    /**
     * Convert the GDF File into a binary file.
     *
     *
     * @param rawGdcFile The gdf file as the user gave it.
     * @param outputFile The output file as the user gave it.
     * @throws IOException Exception.
     * @throws XMLParseException Exception.
     */
    private static void generateFile(File rawGdcFile, File outputFile) throws IOException, XMLParseException {
        File globalDefinitionFile = rawGdcFile.getAbsoluteFile();
        File gdfDirectoryFile = new File(globalDefinitionFile.getParent());

        GlobalDefinitionParser globalDefinitionParser = new GlobalDefinitionParser(globalDefinitionFile.getAbsolutePath(), outputFile.getAbsolutePath());
        File globalDefinitionOutput = globalDefinitionParser.writeBuilder();

        for (String scene : globalDefinitionParser.getGlobalDefinitionBuilder().getScenesList()) {
            SceneDefinitionParser sceneDefinitionParser = new SceneDefinitionParser(gdfDirectoryFile.getAbsolutePath() + "/" + scene + ".sdf",
                    outputFile.getAbsolutePath(), globalDefinitionParser.getSystems());
            sceneDefinitionParser.writeBuilder();
        }

        // Test generated files
        GlobalDefinitionDto.GlobalDefinitionProto.Builder gdfBuilder = GlobalDefinitionDto.GlobalDefinitionProto.newBuilder();
        gdfBuilder.mergeFrom(new FileInputStream(globalDefinitionOutput));
        AssertUtils.makeTest(gdfBuilder.getSystemsCount() > 0);
    }

}
