package fr.kissy.encoder.main.parser;

import fr.kissy.encoder.main.utils.AssertUtils;
import fr.kissy.encoder.main.utils.ParseUtils;
import fr.kissy.encoder.proto.PropertyProto;
import fr.kissy.encoder.proto.SdfProto;
import org.apache.commons.lang3.StringUtils;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.management.modelmbean.XMLParseException;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: SdfParser.java 32 2012-03-29 14:44:19Z kissy $
 */
public class SdfParser extends AbstractParser {
    
    private boolean include = false;
    private List<String> systems;
    private List<String> objects = new ArrayList<String>();

    /**
     * @inheritDoc
     */
    public SdfParser(String xmlPath, String outputPath, List<String> systems) throws XMLParseException {
        super(xmlPath, outputPath);
        this.systems = systems;
    }

    /**
     * @inheritDoc
     */
    public SdfParser(String xmlPath, String outputPath, List<String> systems, boolean include) throws XMLParseException {
        this(xmlPath, outputPath, systems);
        this.include = include;
    }

    /**
     * @inheritDoc
     */
    @Override
    protected void parse() {
        try {
            System.out.println("Parsing SDF File " + xmlFile.getPath());

            builder = SdfProto.Sdf.newBuilder();

            Element documentElement = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(xmlFile).getDocumentElement();
            documentElement.normalize();

            AssertUtils.makeTest("Scene".equals(documentElement.getNodeName()),
                    "SDF files must contain Scene root element");
            
            if (include) {
                getSdfBuidler().setName("Included");
            } else {
                parseScene(documentElement);
            }

            NodeList nodeList = documentElement.getChildNodes();
            for (int i = 0; i < nodeList.getLength(); i++) {
                Node node = nodeList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    if ("Properties".equals(node.getNodeName())) {
                        parseSystemProperties((Element) node);
                    } else if ("Objects".equals(node.getNodeName())) {
                        parseObjects((Element) node);
                    } else if ("Links".equals(node.getNodeName())) {
                        parseLinks((Element) node);
                    } else if ("Include".equals(node.getNodeName())) {
                        parseInclude((Element) node);
                    } else {
                        AssertUtils.makeTest(true,
                                "SDF files must contain Properties, Objects and Links children only");
                    }
                }
            }

            System.out.println("SDF File parsed\n");
            parsed = true;
        } catch (Exception e) {
            throw new RuntimeException("Cannot parse the SDF File " + xmlFile.getPath() + ", " + e.getMessage());
        }
    }

    /**
     * Parse the include element.
     *
     * @param node The include node.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseInclude(Element node) throws XMLParseException {
        SdfParser includeParser = new SdfParser(xmlFile.getParent() + "\\" + ParseUtils.safeGetAttribute(node, "SDF") + ".sdf",
                outputFile.getAbsolutePath(), systems, true);
        SdfProto.Sdf.Builder builder = (SdfProto.Sdf.Builder) includeParser.getBuilder();
        //getSdfBuidler().addAllSystemProperties(builder.getSystemPropertiesList());
        // TODO make system properties update correctly
        getSdfBuidler().addAllObjects(builder.getObjectsList());
        getSdfBuidler().addAllLinks(builder.getLinksList());
    }

    /**
     * Parse the scene.
     * 
     * @param documentElement The document element.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseScene(Element documentElement) throws XMLParseException {
        System.out.println("\t- Parsing Scene");

        getSdfBuidler().setName(ParseUtils.safeGetAttribute(documentElement, "Name"));
    }

    /**
     * Parse the resources.
     *
     * @param propertiesElement The Properties element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void parseSystemProperties(Element propertiesElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        SdfProto.Sdf.SystemProperties.Builder properties = ParseUtils.parseSystemProperties(propertiesElement);
        AssertUtils.makeTest(systems.contains(properties.getSystemType()),
                properties.getSystemType() + " is not a known systemType");

        getSdfBuidler().addSystemProperties(properties);
    }

    /**
     * Parse the objects.
     *
     * @param objectsElement The Objects element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void parseObjects(Element objectsElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        // Store object properties (used because ODF / Object properties can be the same)
        Map<String, SdfProto.Sdf.ObjectProperties> propertiesMap = new HashMap<String, SdfProto.Sdf.ObjectProperties>();

        // Object properties
        NodeList objects = objectsElement.getElementsByTagName("Object");
        for (int i = 0; i < objects.getLength(); i++) {
            SdfProto.Sdf.Object.Builder objectBuilder = SdfProto.Sdf.Object.newBuilder();
            Element objectElement = (Element) objects.item(i);
            
            // Object name
            objectBuilder.setName(ParseUtils.safeGetAttribute(objectElement, "Name"));
            
            // ODF : odf properties will be overridden by object properties
            String odf = objectElement.getAttribute("ODF");
            if (StringUtils.isNotEmpty(odf)) {
                OdfParser odfParser = new OdfParser(xmlFile.getParent() + "\\"  + odf + ".odf",
                        outputFile.getAbsolutePath(), objectBuilder);
                try {
                    odfParser.writeBuilder();
                } catch (IOException ignored) {}
                
                // Fill the properties map
                for (SdfProto.Sdf.ObjectProperties properties : objectBuilder.getObjectPropertiesList()) {
                    propertiesMap.put(properties.getSystemType(), properties);
                }
            }
    
            // Object properties
            NodeList properties = objectElement.getElementsByTagName("Properties");
            for (int j = 0; j < properties.getLength(); j++) {
                SdfProto.Sdf.ObjectProperties.Builder propertiesBuilder = ParseUtils.parseObjectProperties((Element) properties.item(j));
                if (propertiesMap.containsKey(propertiesBuilder.getSystemType())) {
                    List<String> currentSystemProperties = getCurrentSystemProperties(propertiesBuilder.getPropertiesList());
                    for (PropertyProto.Property property : propertiesBuilder.getPropertiesList()) {
                        if (currentSystemProperties.contains(property.getName())) {
                            updateProperty(propertiesBuilder, property);
                        } else {
                            propertiesBuilder.addProperties(property);
                        }
                    }
                } else {
                    objectBuilder.addObjectProperties(propertiesBuilder);
                }
            }
            
            // Add the object to the list
            this.objects.add(objectBuilder.getName());
    
            getSdfBuidler().addObjects(objectBuilder);
        }
    }

    /**
     * Remove the current property.
     * 
     * @param propertiesBuilder The property builder.
     * @param property The property.
     */
    private void updateProperty(SdfProto.Sdf.ObjectProperties.Builder propertiesBuilder, PropertyProto.Property property) {
        for (int i = 0; i < propertiesBuilder.getPropertiesCount(); i++) {
            if (property.getName().equals(propertiesBuilder.getProperties(i).getName())) {
                PropertyProto.Property.Builder propertyBuilder = propertiesBuilder.getProperties(i).toBuilder();
                propertyBuilder.clearValue();
                for (String value : property.getValueList()) {
                    propertyBuilder.addValue(value);
                }
                break;
            }
        }
    }

    /**
     * Get the list of properties name.
     *
     * @param propertiesList The list of Property.
     * @return The list of Property names.
     */
    private List<String> getCurrentSystemProperties(List<PropertyProto.Property> propertiesList) {
        List<String> names = new ArrayList<String>();
        for (PropertyProto.Property property : propertiesList) {
            names.add(property.getName());
        }
        return names;
    }

    /**
     * Parse the links.
     * 
     * @param linksElement The Links element.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseLinks(Element linksElement) throws XMLParseException {
        System.out.println("\t- Parsing Links");

        // Object properties
        NodeList links = linksElement.getElementsByTagName("Link");
        for (int i = 0; i < links.getLength(); i++) {
            SdfProto.Sdf.Link.Builder linkBuilder = SdfProto.Sdf.Link.newBuilder();
            Element linkElement = (Element) links.item(i);
            
            linkBuilder.setSubject(ParseUtils.safeGetAttribute(linkElement, "Subject"));
            linkBuilder.setObserver(ParseUtils.safeGetAttribute(linkElement, "Observer"));
            linkBuilder.setSubjectSystem(ParseUtils.safeGetAttribute(linkElement, "SubjectSystem"));
            linkBuilder.setObserverSystem(ParseUtils.safeGetAttribute(linkElement, "ObserverSystem"));

            AssertUtils.makeTest(objects.contains(linkBuilder.getSubject()),
                    linkBuilder.getSubject() + " is not a known object");
            AssertUtils.makeTest(objects.contains(linkBuilder.getObserver()),
                    linkBuilder.getObserver() + " is not a known object");
            AssertUtils.makeTest(systems.contains(linkBuilder.getSubjectSystem()),
                    linkBuilder.getSubjectSystem() + " is not a known systemType");
            AssertUtils.makeTest(systems.contains(linkBuilder.getObserverSystem()),
                    linkBuilder.getObserverSystem() + " is not a known systemType");
            
            getSdfBuidler().addLinks(linkBuilder);
        }
    }

    /**
     * Get the SDF Builder.
     *
     * @return The SDF Builder.
     */
    public SdfProto.Sdf.Builder getSdfBuidler() {
        return (SdfProto.Sdf.Builder) builder;
    }
}
