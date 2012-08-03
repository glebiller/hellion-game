package fr.kissy.encoder.main.utils;

import fr.kissy.encoder.proto.PropertyProto;
import fr.kissy.encoder.proto.SdfProto;
import org.apache.commons.lang3.BooleanUtils;
import org.apache.commons.lang3.StringUtils;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import javax.management.modelmbean.XMLParseException;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: ParseUtils.java 8 2012-03-04 20:36:01Z Kissy $
 */
public class ParseUtils {
    private static final CharSequence STRING_1 = "1";

    /**
     * Parse a property item.
     *
     * @param propertyElement The property node to parse.
     * @return The property builder created.
     * @throws javax.management.modelmbean.XMLParseException If the xml is not well formatted.
     */
    public static PropertyProto.Property.Builder parseProperty(Element propertyElement) throws XMLParseException {
        PropertyProto.Property.Builder propertyBuilder = PropertyProto.Property.newBuilder();
        NamedNodeMap attributes = propertyElement.getAttributes();
        
        String name = propertyElement.getAttribute("Name");
        if (StringUtils.isEmpty(name)) {
            AssertUtils.makeTest(false,
                    "Name attribute is required for Property element");
        }
        propertyBuilder.setName(name);
        
        for (int j = 0; j < attributes.getLength(); j++) {
            String attribute = attributes.item(j).getNodeName();
            if (!attribute.equals("Name")) {
                propertyBuilder.addValue(propertyElement.getAttribute(attribute));
            }
        }
        return propertyBuilder;
    }

    /**
     * Parse the system properties.
     *
     * @param propertiesElement The property element.
     * @return The group properties created.
     * @throws javax.management.modelmbean.XMLParseException If the xml is not well formatted.
     */
    public static SdfProto.Sdf.SystemProperties.Builder parseSystemProperties(Element propertiesElement) throws XMLParseException {
        // System type
        SdfProto.Sdf.SystemProperties.Builder systemProperties = SdfProto.Sdf.SystemProperties.newBuilder();

        systemProperties.setSystemType(safeGetAttribute(propertiesElement, "SystemType"));

        // System properties
        NodeList properties = propertiesElement.getElementsByTagName("Property");
        for (int i = 0; i < properties.getLength(); i++) {
            systemProperties.addProperties(ParseUtils.parseProperty((Element) properties.item(i)));
        }

        return systemProperties;
    }

    /**
     * Parse the object properties.
     *
     * @param propertiesElement The property element.
     * @return The group properties created.
     * @throws javax.management.modelmbean.XMLParseException If the xml is not well formatted.
     */
    public static SdfProto.Sdf.ObjectProperties.Builder parseObjectProperties(Element propertiesElement) throws XMLParseException {
        // System type
        SdfProto.Sdf.ObjectProperties.Builder objectProperties = SdfProto.Sdf.ObjectProperties.newBuilder();

        objectProperties.setSystemType(safeGetAttribute(propertiesElement, "SystemType"));
        String objectType = propertiesElement.getAttribute("ObjectType");
        if (StringUtils.isNotEmpty(objectType)) {
            objectProperties.setObjectType(objectType);
        }

        // System properties
        NodeList properties = propertiesElement.getElementsByTagName("Property");
        for (int i = 0; i < properties.getLength(); i++) {
            objectProperties.addProperties(ParseUtils.parseProperty((Element) properties.item(i)));
        }

        return objectProperties;
    }

    /**
     * Safe get an attribute.
     *
     * @param element The element.
     * @param attribute The attribute name.
     * @return The string.
     * @throws XMLParseException The exception.
     */
    public static String safeGetAttribute(Element element, String attribute) throws XMLParseException {
        String value = element.getAttribute(attribute);
        AssertUtils.makeTest(StringUtils.isNotEmpty(value),
                attribute + " attribute is required for " + element.getNodeName() + " element");
        return value;
    }

    /**
     * Parse a boolean value.
     * Return true if value is equals to
     * "true" "TRUE" "yes" "on" "1"
     *
     * @param value The value to parse.
     * @return The boolean parsed.
     */
    public static boolean parseBoolean(String value) {
        return BooleanUtils.toBoolean(value) || StringUtils.equals(value, STRING_1);
    }
}
