package fr.kissy.hellion.definition.encoder.main.parser;

import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;
import fr.kissy.hellion.definition.encoder.main.utils.ParseUtils;
import fr.kissy.hellion.proto.Common;
import fr.kissy.hellion.proto.Definition;
import org.apache.commons.beanutils.MethodUtils;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.management.modelmbean.XMLParseException;
import javax.xml.parsers.DocumentBuilderFactory;
import java.util.ArrayList;
import java.util.List;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: ApplicationParser.java 32 2012-03-29 14:44:19Z kissy $
 */
public class ApplicationParser extends AbstractParser {
    
    private List<Common.SystemType> systems = new ArrayList<>();

    /**
     * @inheritDoc
     */
    public ApplicationParser(String xmlPath, String outputPath) throws XMLParseException {
        super(xmlPath, outputPath);
    }

    /**
     * @inheritDoc
     */
    @Override
    protected void parse() {
        try {
            System.out.println("Parsing ADF File " + xmlFile.getPath());

            builder = Definition.Application.newBuilder();

            Element documentElement = DocumentBuilderFactory.newInstance().newDocumentBuilder()
                    .parse(xmlFile).getDocumentElement();
            documentElement.normalize();

            AssertUtils.makeTest("Application".equals(documentElement.getNodeName()), "ADF files must contain Application root element");

            NodeList nodeList = documentElement.getChildNodes();
            for (int i = 0; i < nodeList.getLength(); i++) {
                Node node = nodeList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    MethodUtils.invokeExactMethod(this, "parse" + node.getNodeName(), new Object[]{node}, new Class[]{Element.class});
                }
            }

            System.out.println("ADF File parsed\n");
            parsed = true;
        } catch (Exception e) {
            throw new RuntimeException("Cannot parse the ADF File " + xmlFile.getPath() + ", " + e.getMessage());
        }
    }

    /**
     * Parse the environment.
     *
     * @param propertiesElement The properties element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    public void parseProperties(Element propertiesElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");

        NodeList properties = propertiesElement.getElementsByTagName("Property");
        for (int i = 0; i < properties.getLength(); i++) {
            getApplicationBuilder().addProperties(ParseUtils.parseProperty((Element) properties.item(i)));
        }
    }

    /**
     * Parse the systems.
     *
     * @param systemsElement The Systems element.
     * @throws XMLParseException Exception if the document is malformed.
     */
    public void parseSystems(Element systemsElement) throws XMLParseException {
        System.out.println("\t- Parsing Systems");

        NodeList systemList = systemsElement.getElementsByTagName("System");
        for (int i = 0; i < systemList.getLength(); i++) {
            Common.System.Builder systemBuilder = Common.System.newBuilder();
            Element systemElement = (Element) systemList.item(i);

            String type = ParseUtils.safeGetAttribute(systemElement, "Type");
            systemBuilder.setType(Common.SystemType.valueOf(type));

            NodeList properties = systemElement.getElementsByTagName("Property");
            for (int j = 0; j < properties.getLength(); j++) {
                systemBuilder.addProperties(ParseUtils.parseProperty((Element) properties.item(j)));
            }

            // Save the system into the list.
            systems.add(systemBuilder.getType());

            getApplicationBuilder().addSystems(systemBuilder);
        }
    }

    /**
     * Parse the scenes.
     *
     * @param scenesElement The Scenes element.
     * @throws XMLParseException Exception if the document is malformed.
     */
    public void parseScenes(Element scenesElement) throws XMLParseException {
        System.out.println("\t- Parsing Scenes");

        getApplicationBuilder().setStartupScene(ParseUtils.safeGetAttribute(scenesElement, "Startup"));

        // Scenes
        NodeList scenesList = scenesElement.getElementsByTagName("Scene");
        for (int i = 0; i < scenesList.getLength(); i++) {
            Element sceneElement = (Element) scenesList.item(i);
            getApplicationBuilder().addScenes(ParseUtils.safeGetAttribute(sceneElement, "Name"));
        }
    }

    /**
     * Get the ADF Builder.
     *
     * @return The ADF Builder.
     */
    public Definition.Application.Builder getApplicationBuilder() {
        return (Definition.Application.Builder) builder;
    }

    /**
     * Get the system list.
     *
     * @return The system list.
     */
    public List<Common.SystemType> getSystems() {
        return systems;
    }
}
