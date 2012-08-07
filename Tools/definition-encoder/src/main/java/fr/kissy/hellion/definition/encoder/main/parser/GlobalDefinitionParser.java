package fr.kissy.hellion.definition.encoder.main.parser;

import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;
import fr.kissy.hellion.definition.encoder.main.utils.ParseUtils;
import fr.kissy.hellion.definition.encoder.proto.GlobalDefinitionProto;
import fr.kissy.hellion.definition.encoder.proto.SystemProto;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.management.modelmbean.XMLParseException;
import javax.xml.parsers.DocumentBuilderFactory;
import java.util.ArrayList;
import java.util.List;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: GlobalDefinitionParser.java 32 2012-03-29 14:44:19Z kissy $
 */
public class GlobalDefinitionParser extends AbstractParser {
    
    private List<SystemProto.System.Type> systems = new ArrayList<SystemProto.System.Type>();

    /**
     * @inheritDoc
     */
    public GlobalDefinitionParser(String xmlPath, String outputPath) throws XMLParseException {
        super(xmlPath, outputPath);
    }

    /**
     * @inheritDoc
     */
    @Override
    protected void parse() {
        try {
            System.out.println("Parsing GDF File " + xmlFile.getPath());

            builder = GlobalDefinitionProto.GlobalDefinition.newBuilder();

            Element documentElement = DocumentBuilderFactory.newInstance().newDocumentBuilder()
                    .parse(xmlFile).getDocumentElement();
            documentElement.normalize();

            AssertUtils.makeTest("GlobalDefinition".equals(documentElement.getNodeName()),
                    "GDF files must contain GlobalDefinition root element");

            NodeList nodeList = documentElement.getChildNodes();
            for (int i = 0; i < nodeList.getLength(); i++) {
                Node node = nodeList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    if ("Properties".equals(node.getNodeName())) {
                        parseProperties((Element) node);
                    } else if ("Systems".equals(node.getNodeName())) {
                        parseSystems((Element) node);
                    } else if ("Scenes".equals(node.getNodeName())) {
                        parseScenes((Element) node);
                    } else {
                        AssertUtils.makeTest(false,
                                "GDF files must contain Environment, Systems and Scenes children only");
                    }
                }
            }

            System.out.println("GDF File parsed\n");
            parsed = true;
        } catch (Exception e) {
            throw new RuntimeException("Cannot parse the GDF File " + xmlFile.getPath() + ", " + e.getMessage());
        }
    }

    /**
     * Parse the environment.
     *
     * @param propertiesElement The properties element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void parseProperties(Element propertiesElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        NodeList properties = propertiesElement.getElementsByTagName("Property");
        for (int i = 0; i < properties.getLength(); i++) {
            getGlobalDefinitionBuilder().addProperties(ParseUtils.parseProperty((Element) properties.item(i)));
        }
    }

    /**
     * Parse the systems.
     *
     * @param systemsElement The Systems element.
     * @throws XMLParseException Exception if the document is malformed.
     */
    private void parseSystems(Element systemsElement) throws XMLParseException {
        System.out.println("\t- Parsing Systems");

        NodeList systemList = systemsElement.getElementsByTagName("System");
        for (int i = 0; i < systemList.getLength(); i++) {
            SystemProto.System.Builder systemBuilder = SystemProto.System.newBuilder();
            Element systemElement = (Element) systemList.item(i);

            String type = ParseUtils.safeGetAttribute(systemElement, "Type");
            systemBuilder.setType(SystemProto.System.Type.valueOf(type));

            NodeList properties = systemElement.getElementsByTagName("Property");
            for (int j = 0; j < properties.getLength(); j++) {
                systemBuilder.addProperties(ParseUtils.parseProperty((Element) properties.item(j)));
            }

            // Save the system into the list.
            systems.add(systemBuilder.getType());

            getGlobalDefinitionBuilder().addSystems(systemBuilder);
        }
    }

    /**
     * Parse the scenes.
     *
     * @param scenesElement The Scenes element.
     * @throws XMLParseException Exception if the document is malformed.
     */
    private void parseScenes(Element scenesElement) throws XMLParseException {
        System.out.println("\t- Parsing Scenes");

        getGlobalDefinitionBuilder().setStartupScene(ParseUtils.safeGetAttribute(scenesElement, "Startup"));

        // Scenes
        NodeList scenesList = scenesElement.getElementsByTagName("Scene");
        for (int i = 0; i < scenesList.getLength(); i++) {
            Element sceneElement = (Element) scenesList.item(i);
            getGlobalDefinitionBuilder().addScenes(ParseUtils.safeGetAttribute(sceneElement, "Name"));
        }
    }

    /**
     * Get the GDF Builder.
     *
     * @return The GDF Builder.
     */
    public GlobalDefinitionProto.GlobalDefinition.Builder getGlobalDefinitionBuilder() {
        return (GlobalDefinitionProto.GlobalDefinition.Builder) builder;
    }

    /**
     * Get the system list.
     *
     * @return The system list.
     */
    public List<SystemProto.System.Type> getSystems() {
        return systems;
    }
}
