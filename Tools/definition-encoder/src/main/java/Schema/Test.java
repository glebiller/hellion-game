package Schema;

import com.google.flatbuffers.FlatBufferBuilder;

import java.io.DataOutputStream;
import java.io.FileOutputStream;

/**
 * @author Guillaume Le Biller (<i>guillaume.lebiller@gmail.com</i>)
 */
public class Test {
    public static void main(String[] args) {
        FlatBufferBuilder fbb = new FlatBufferBuilder(1);

        int str = fbb.createString("NameA");

        GraphicSystem.startGraphicSystem(fbb);
        GraphicSystem.addNamea(fbb, str);
        int graphicSystem = GraphicSystem.endGraphicSystem(fbb);

        Application.startSystemsTestVector(fbb, 1);
        Systems.createSystems(fbb, SystemsType.GraphicSystem, graphicSystem);
        int systemsVec = fbb.endVector();

        Application.startApplication(fbb);
        Application.addSystemsTest(fbb, systemsVec);
        int i1 = Application.endApplication(fbb);

        Application.finishApplicationBuffer(fbb, i1);

        try {
            DataOutputStream os = new DataOutputStream(new FileOutputStream("../test.bin"));
            os.write(fbb.dataBuffer().array(), fbb.dataBuffer().position(), fbb.offset());
            os.close();
        } catch(java.io.IOException e) {
        }
    }
}
