package fr.kissy.hellion.definition.encoder.main.parser;

import com.google.protobuf.ByteString;
import fr.kissy.hellion.definition.encoder.main.utils.AssertUtils;
import fr.kissy.hellion.definition.encoder.main.utils.ParseUtils;
import fr.kissy.hellion.proto.Common;
import fr.kissy.hellion.proto.Definition;
import org.apache.commons.beanutils.MethodUtils;
import org.apache.commons.lang3.StringUtils;
import org.bson.types.ObjectId;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.management.modelmbean.XMLParseException;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author Guillaume Le Biller <lebiller@ekino.com>
 * @version $Id: SceneParser.java 32 2012-03-29 14:44:19Z kissy $
 */
public class SceneParser extends AbstractParser {
    
    private boolean include = false;
    private List<Common.SystemType> systems;
    private List<String> objects = new ArrayList<>();

    /**
     * @inheritDoc
     */
    public SceneParser(String xmlPath, String outputPath, List<Common.SystemType> systems) throws XMLParseException {
        super(xmlPath, outputPath);
        this.systems = systems;
    }

    /**
     * @inheritDoc
     */
    public SceneParser(String xmlPath, String outputPath, List<Common.SystemType> systems, boolean include) throws XMLParseException {
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

            builder = Definition.Scene.newBuilder();

            Element documentElement = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(xmlFile).getDocumentElement();
            documentElement.normalize();

            AssertUtils.makeTest("Scene".equals(documentElement.getNodeName()), "SDF files must contain Scene root element");
            
            if (include) {
                getSceneDefinitionBuilder().setName("Included");
            } else {
                parseScene(documentElement);
            }

            NodeList nodeList = documentElement.getChildNodes();
            for (int i = 0; i < nodeList.getLength(); i++) {
                Node node = nodeList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    MethodUtils.invokeExactMethod(this, "parse" + node.getNodeName(), new Object[] {node}, new Class[] {Element.class});
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
    public void parseInclude(Element node) throws XMLParseException {
        SceneParser includeParser = new SceneParser(xmlFile.getParent() + "\\" + ParseUtils.safeGetAttribute(node, "SDF") + ".sdf",
                outputFile.getAbsolutePath(), systems, true);
        Definition.Scene.Builder builder = (Definition.Scene.Builder) includeParser.getBuilder();
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
    public void parseScene(Element documentElement) throws XMLParseException {
        System.out.println("\t- Parsing Scene");

        getSceneDefinitionBuilder().setName(ParseUtils.safeGetAttribute(documentElement, "Name"));
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

            Common.SystemType type = Common.SystemType.valueOf(ParseUtils.safeGetAttribute(systemElement, "Type"));
            systemBuilder.setType(type);
            AssertUtils.makeTest(this.systems.contains(type), type + " is not a known systemType");

            NodeList properties = systemElement.getElementsByTagName("Property");
            for (int j = 0; j < properties.getLength(); j++) {
                systemBuilder.addProperties(ParseUtils.parseProperty((Element) properties.item(j)));
            }

            // Save the system into the list.
            systems.add(systemBuilder.getType());

            getSceneDefinitionBuilder().addSystems(systemBuilder);
        }
    }

    /**
     * Parse the objects.
     *
     * @param objectsElement The Objects element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    public void parseObjects(Element objectsElement) throws XMLParseException {
        System.out.println("\t- Parsing Properties");
        internalParseObjects(objectsElement, true);
    }

    /**
     * Parse the templates.
     *
     * @param templatesElement The Templates element.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    public void parseTemplates(Element templatesElement) throws XMLParseException {
        System.out.println("\t- Parsing Templates");
        internalParseObjects(templatesElement, false);
    }

    /**
     * Parse all Object elements contained either in Objects or Templates elements.
     *
     * @param parentElement The element containing Object elements.
     * @param object True if we are parsing real objects, false if we are parsing templates.
     * @throws javax.management.modelmbean.XMLParseException Exception if the document is malformed.
     */
    private void internalParseObjects(Element parentElement, boolean object) throws XMLParseException {
        // Object properties
        NodeList objects = parentElement.getElementsByTagName("Object");
        for (int i = 0; i < objects.getLength(); i++) {
            Common.Object.Builder objectBuilder = Common.Object.newBuilder();
            Element objectElement = (Element) objects.item(i);

            // Object name
            String id = objectElement.getAttribute("Id");
            objectBuilder.setId(StringUtils.isNotBlank(id) ? id : new ObjectId().toString());
            objectBuilder.setName(ParseUtils.safeGetAttribute(objectElement, "Name"));
            objectBuilder.setParent(objectElement.getAttribute("Parent"));
            objectBuilder.setTemplate(objectElement.getAttribute("Template"));

            // Object properties
            NodeList properties = objectElement.getElementsByTagName("Properties");
            for (int j = 0; j < properties.getLength(); j++) {
                Common.SystemObject.Builder propertiesBuilder = ParseUtils.parseObjectProperties((Element) properties.item(j));
                objectBuilder.addSystemObjects(propertiesBuilder);
            }

            if (object) {
                // Add the object to the list
                this.objects.add(objectBuilder.getName());
                getSceneDefinitionBuilder().addObjects(objectBuilder);
            } else {
                getSceneDefinitionBuilder().addTemplates(objectBuilder);
            }
        }
    }

    /**
     * Parse the links.
     *
     * @param linksElement The Links element.
     * @throws javax.management.modelmbean.XMLParseException The exception.
     */
    public void parseLinks(Element linksElement) throws XMLParseException {
        System.out.println("\t- Parsing Links");

        // Object properties
        NodeList links = linksElement.getElementsByTagName("Link");
        for (int i = 0; i < links.getLength(); i++) {
            Definition.Link.Builder linkBuilder = Definition.Link.newBuilder();
            Element linkElement = (Element) links.item(i);

            linkBuilder.setSubject(ParseUtils.safeGetAttribute(linkElement, "Subject"));
            linkBuilder.setObserver(ParseUtils.safeGetAttribute(linkElement, "Observer"));
            String subjectSystemType = ParseUtils.safeGetAttribute(linkElement, "SubjectSystem");
            linkBuilder.setSubjectSystemType(Common.SystemType.valueOf(subjectSystemType));
            String observerSystemType = ParseUtils.safeGetAttribute(linkElement, "ObserverSystem");
            linkBuilder.setObserverSystemType(Common.SystemType.valueOf(observerSystemType));

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
    public Definition.Scene.Builder getSceneDefinitionBuilder() {
        return (Definition.Scene.Builder) builder;
    }
}
