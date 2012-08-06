package fr.kissy.encoder.main;

import fr.kissy.encoder.main.parser.GdfParser;
import fr.kissy.encoder.main.parser.SdfParser;
import fr.kissy.encoder.main.utils.AssertUtils;
import fr.kissy.encoder.proto.GdfProto;

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
        File gdfFile = new File(gdfPath);
        if (!gdfFile.exists() || !gdfFile.canRead()) {
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
        generateFile(gdfFile, outputFile);
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
        File gdfFile = rawGdcFile.getAbsoluteFile();
        File gdfDirectoryFile = new File(gdfFile.getParent());

        GdfParser gdfParser = new GdfParser(gdfFile.getAbsolutePath(), outputFile.getAbsolutePath());
        File gdfProto = gdfParser.writeBuilder();

        for (String scene : gdfParser.getGdfBuidler().getScenesList()) {
            SdfParser sdfParser = new SdfParser(gdfDirectoryFile.getAbsolutePath() + "/" + scene + ".sdf",
                    outputFile.getAbsolutePath(), gdfParser.getSystems());
            sdfParser.writeBuilder();
        }

        // Test generated files
        GdfProto.Gdf.Builder gdfBuilder = GdfProto.Gdf.newBuilder();
        gdfBuilder.mergeFrom(new FileInputStream(gdfProto));
        AssertUtils.makeTest(gdfBuilder.getSystemsCount() > 0);
    }

}
