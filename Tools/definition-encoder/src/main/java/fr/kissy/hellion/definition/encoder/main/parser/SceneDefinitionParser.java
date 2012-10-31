package fr.kissy.hellion.definition.encoder.main.parser;

import com.google.protobuf.ByteString;
import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;
import fr.kissy.hellion.definition.encoder.main.utils.ParseUtils;
import fr.kissy.hellion.proto.common.ObjectDto;
import fr.kissy.hellion.proto.common.PropertyDto;
import fr.kissy.hellion.proto.common.SystemDto;
import fr.kissy.hellion.proto.definition.SceneDefinitionDto;
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
 * @version $Id: SceneDefinitionParser.java 32 2012-03-29 14:44:19Z kissy $
 */
public class SceneDefinitionParser extends AbstractParser {
    
    private boolean include = false;
    private List<SystemDto.SystemProto.Type> systems;
    private List<String> objects = new ArrayList<String>();

    /**
     * @inheritDoc
     */
    public SceneDefinitionParser(String xmlPath, String outputPath, List<SystemDto.SystemProto.Type> systems) throws XMLParseException {
        super(xmlPath, outputPath);
        this.systems = systems;
    }

    /**
     * @inheritDoc
     */
    public SceneDefinitionParser(String xmlPath, String outputPath, List<SystemDto.SystemProto.Type> systems, boolean include) throws XMLParseException {
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

            builder = SceneDefinitionDto.SceneDefinitionProto.newBuilder();

            Element documentElement = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(xmlFile).getDocumentElement();
            documentElement.normalize();

            AssertUtils.makeTest("Scene".equals(documentElement.getNodeName()),
                    "SDF files must contain Scene root element");
            
            if (include) {
                getSceneDefinitionBuilder().setName("Included");
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
        SceneDefinitionParser includeParser = new SceneDefinitionParser(xmlFile.getParent() + "\\" + ParseUtils.safeGetAttribute(node, "SDF") + ".sdf",
                outputFile.getAbsolutePath(), systems, true);
        SceneDefinitionDto.SceneDefinitionProto.Builder builder = (SceneDefinitionDto.SceneDefinitionProto.Builder) includeParser.getBuilder();
        // TODO make system properties update correctly
        getSceneDefinitionBuilder().addAllObjects(builder.getObjectsList());
        getSceneDefinitionBuilder().addAllLinks(builder.getLinksList());
    }

    /**
     * Parse the scene.
     * 
     * @param documentElement The document element.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    private void parseScene(Element documentElement) throws XMLParseException {
        System.out.println("\t- Parsing Scene");

        getSceneDefinitionBuilder().setName(ParseUtils.safeGetAttribute(documentElement, "Name"));
    }

    /**
     * Parse the resources.
     *
     * @param propertiesElement The Properties element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void parseSystemProperties(Element propertiesElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        SystemDto.SystemProto.Builder systems = ParseUtils.parseSystemProperties(propertiesElement);
        AssertUtils.makeTest(this.systems.contains(systems.getType()), systems.getType() + " is not a known systemType");

        getSceneDefinitionBuilder().addSystems(systems);
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
        Map<String, ObjectDto.ObjectProto.SystemObjectProto> propertiesMap = new HashMap<String, ObjectDto.ObjectProto.SystemObjectProto>();

        // Object properties
        NodeList objects = objectsElement.getElementsByTagName("Object");
        for (int i = 0; i < objects.getLength(); i++) {
            ObjectDto.ObjectProto.Builder objectBuilder = ObjectDto.ObjectProto.newBuilder();
            Element objectElement = (Element) objects.item(i);
            
            // Object name
            objectBuilder.setName(ParseUtils.safeGetAttribute(objectElement, "Name"));
            
            // ODF : odf properties will be overridden by object properties
            String odf = objectElement.getAttribute("ODF");
            if (StringUtils.isNotEmpty(odf)) {
                ObjectDefinitionParser objectDefinitionParser = new ObjectDefinitionParser(xmlFile.getParent() + "\\"  + odf + ".odf",
                        outputFile.getAbsolutePath(), objectBuilder);
                try {
                    objectDefinitionParser.writeBuilder();
                } catch (IOException ignored) {}
                
                // Fill the properties map
                for (ObjectDto.ObjectProto.SystemObjectProto properties : objectBuilder.getSystemObjectsList()) {
                    propertiesMap.put(properties.getType(), properties);
                }
            }
    
            // Object properties
            NodeList properties = objectElement.getElementsByTagName("Properties");
            for (int j = 0; j < properties.getLength(); j++) {
                ObjectDto.ObjectProto.SystemObjectProto.Builder propertiesBuilder = ParseUtils.parseObjectProperties((Element) properties.item(j));
                if (propertiesMap.containsKey(propertiesBuilder.getType())) {
                    List<String> currentSystemProperties = getCurrentSystemProperties(propertiesBuilder.getPropertiesList());
                    for (PropertyDto.PropertyProto property : propertiesBuilder.getPropertiesList()) {
                        if (currentSystemProperties.contains(property.getName())) {
                            updateProperty(propertiesBuilder, property);
                        } else {
                            propertiesBuilder.addProperties(property);
                        }
                    }
                } else {
                    objectBuilder.addSystemObjects(propertiesBuilder);
                }
            }
            
            // Add the object to the list
            this.objects.add(objectBuilder.getName());
    
            getSceneDefinitionBuilder().addObjects(objectBuilder);
        }
    }

    /**
     * Remove the current property.
     * 
     * @param propertiesBuilder The property builder.
     * @param property The property.
     */
    private void updateProperty(ObjectDto.ObjectProto.SystemObjectProto.Builder propertiesBuilder, PropertyDto.PropertyProto property) {
        for (int i = 0; i < propertiesBuilder.getPropertiesCount(); i++) {
            if (property.getName().equals(propertiesBuilder.getProperties(i).getName())) {
                PropertyDto.PropertyProto.Builder propertyBuilder = propertiesBuilder.getProperties(i).toBuilder();
                propertyBuilder.clearValue();
                for (ByteString value : property.getValueList()) {
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
    private List<String> getCurrentSystemProperties(List<PropertyDto.PropertyProto> propertiesList) {
        List<String> names = new ArrayList<String>();
        for (PropertyDto.PropertyProto property : propertiesList) {
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
            SceneDefinitionDto.SceneDefinitionProto.LinkProto.Builder linkBuilder = SceneDefinitionDto.SceneDefinitionProto.LinkProto.newBuilder();
            Element linkElement = (Element) links.item(i);
            
            linkBuilder.setSubject(ParseUtils.safeGetAttribute(linkElement, "Subject"));
            linkBuilder.setObserver(ParseUtils.safeGetAttribute(linkElement, "Observer"));
            String subjectSystemType = ParseUtils.safeGetAttribute(linkElement, "SubjectSystem");
            linkBuilder.setSubjectSystemType(SystemDto.SystemProto.Type.valueOf(subjectSystemType));
            String observerSystemType = ParseUtils.safeGetAttribute(linkElement, "ObserverSystem");
            linkBuilder.setObserverSystemType(SystemDto.SystemProto.Type.valueOf(observerSystemType));

            AssertUtils.makeTest(objects.contains(linkBuilder.getSubject()),
                    linkBuilder.getSubject() + " is not a known object");
            AssertUtils.makeTest(objects.contains(linkBuilder.getObserver()),
                    linkBuilder.getObserver() + " is not a known object");
            AssertUtils.makeTest(systems.contains(linkBuilder.getSubjectSystemType()),
                    linkBuilder.getSubjectSystemType() + " is not a known systemType");
            AssertUtils.makeTest(systems.contains(linkBuilder.getObserverSystemType()),
                    linkBuilder.getObserverSystemType() + " is not a known systemType");
            
            getSceneDefinitionBuilder().addLinks(linkBuilder);
        }
    }

    /**
     * Get the SDF Builder.
     *
     * @return The SDF Builder.
     */
    public SceneDefinitionDto.SceneDefinitionProto.Builder getSceneDefinitionBuilder() {
        return (SceneDefinitionDto.SceneDefinitionProto.Builder) builder;
    }
}
