package fr.kissy.hellion.definition.encoder.main.parser;

import com.google.protobuf.GeneratedMessage;
import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;

import javax.management.modelmbean.XMLParseException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: AbstractParser.java 16 2012-03-11 23:19:09Z Kissy $
 */
public abstract class AbstractParser {
    protected GeneratedMessage.Builder builder;
    protected boolean parsed;
    protected File xmlFile;
    protected File outputFile;

    /**
     * Default constructor.
     *
     * @param xmlPath The path to the XML file.
     * @param outputPath The path to output the bin file.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    public AbstractParser(String xmlPath, String outputPath) throws XMLParseException {
        parsed = false;
        xmlFile = new File(xmlPath);
        outputFile = new File(outputPath);
        AssertUtils.makeTest(xmlFile.exists() && xmlFile.canRead(),
                "The xml file " + xmlFile.getAbsolutePath() + " does not exists or cannot be read.");
    }

    /**
     * Parse the File.
     */
    protected abstract void parse();

    /**
     * Get the Builder.
     *
     * @return The Builder.
     */
    public GeneratedMessage.Builder getBuilder() {
        if (!parsed) {
            parse();
        }
        return builder;
    }

    /**
     * Write the parsed file to Protobuf file.
     *
     * @throws java.io.FileNotFoundException The exception.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     * @return The generated file.
     */
    public File writeBuilder() throws IOException, XMLParseException {
        if (builder == null || !builder.isInitialized()) {
            parse();
        }

        File proto = new File(outputFile.getAbsolutePath() + "/" + xmlFile.getName() + ".bin");
        File parent = new File(proto.getParent());
        
        if (!parent.exists()) {
            AssertUtils.makeTest(parent.mkdirs(), "Fail to create directory " + parent.getAbsolutePath());
        }
        if (proto.exists()) {
            AssertUtils.makeTest(proto.delete(), "Fail to delete the file " + proto.getAbsolutePath());
        }
        
        if (proto.createNewFile() && proto.canWrite()) {
            builder.build().writeTo(new FileOutputStream(proto));
            System.out.println("Output bin file to " + proto.getAbsolutePath() + "\n");
        } else {
            AssertUtils.makeTest(false, "Fail to write the file " + proto.getAbsolutePath());
        }

        return proto;
    }

}
