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
        if (args.length == 0) {
            System.out.println("First argument must be the file to process");
            return;
        }
        String gdfPath = args[0];
        
        // Test if gdf file exists and find the directory.
        File gdfFile = new File(gdfPath);
        if (!gdfFile.exists() || !gdfFile.canRead()) {
            System.out.println("File " + gdfPath + " not found.");
            return;
        }
        gdfFile = gdfFile.getAbsoluteFile();
        File gdfDirectoryFile = new File(gdfFile.getParent());
        
        GdfParser gdfParser = new GdfParser(gdfFile.getAbsolutePath());
        File gdfProto = gdfParser.writeBuilder();
        
        for (GdfProto.Gdf.Scene scene : gdfParser.getGdfBuidler().getScenesList()) {
            SdfParser sdfParser = new SdfParser(gdfDirectoryFile.getAbsolutePath() + "\\" + scene.getSdf() + ".sdf",
                    gdfParser.getSystems());
            sdfParser.writeBuilder();
        }

        // Test generated files
        GdfProto.Gdf.Builder gdfBuilder = GdfProto.Gdf.newBuilder();
        gdfBuilder.mergeFrom(new FileInputStream(gdfProto));
        AssertUtils.makeTest(gdfBuilder.getSystemsCount() > 0);
    }

}
