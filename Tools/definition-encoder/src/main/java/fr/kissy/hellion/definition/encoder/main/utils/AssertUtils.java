package fr.kissy.hellion.definition.encoder.main.utils;

import org.apache.commons.lang3.StringUtils;

import javax.management.modelmbean.XMLParseException;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: AssertUtils.java 7 2012-03-02 17:43:54Z kissy $
 */
public class AssertUtils {

    /**
     * Throw exception is condition is false.
     *
     * @param condition The condition to test.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    public static void makeTest(boolean condition) throws XMLParseException {
        makeTest(condition, null);
    }

    /**
     * Throw exception is condition is false.
     * 
     * @param condition The condition to test.
     * @param message The message to display.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    public static void makeTest(boolean condition, String message) throws XMLParseException {
        if (!condition) {
            String debug = message == null ? StringUtils.EMPTY : " : " + message;
            throw new XMLParseException("Error while parsing the XML" + debug);
        }
    }
}
