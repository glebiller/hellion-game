/*
 * Copyright (c) 2009 Stephen Tu <stephen_tu@berkeley.edu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package fr.kissy.helion.client;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import fr.kissy.helion.client.rpc.NettyRpcChannel;
import fr.kissy.helion.client.rpc.NettyRpcClient;
import fr.kissy.helion.server.proto.service.Authentification;
import org.apache.log4j.BasicConfigurator;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

public class TestMain {

	public static void main(String[] args) {
		
		BasicConfigurator.configure();
		
		NettyRpcClient client = new NettyRpcClient(
				new NioClientSocketChannelFactory(
						Executors.newCachedThreadPool(), 
						Executors.newCachedThreadPool()));
		
		NettyRpcChannel channel = client.blockingConnect(new InetSocketAddress("localhost", 8080));

        Authentification.AuthentificationService authentificationService
                = Authentification.AuthentificationService.newStub(channel);
        Authentification.AuthentificationService.BlockingInterface blockingAuthentificationService
                = Authentification.AuthentificationService.newBlockingStub(channel);
		
		// Get a new RpcController to use for this rpc call
		final RpcController controller = channel.newRpcController();
		
		// Create the request
        Authentification.LogonRequest request = Authentification.LogonRequest.newBuilder()
                .setUsername("test").setPassword("pass").build();

		// Make the (asynchronous) RPC request
        authentificationService.login(controller, request, new RpcCallback<Authentification.LogonResponse>() {
			public void run(Authentification.LogonResponse response) {
				if (response != null) {
					System.out.println("The login status is: " + response.getStatus());
				} else { 
					System.out.println("Oops, there was an error: " + controller.errorText());
				}
			}
		});
		
		// Do other important things now, while your RPC is hard at work
		try {
			Thread.sleep(1000L);
		} catch (InterruptedException e) {
			// Ignore
		}
		
		// Reset the controller
		controller.reset();

		// Close the channel
		channel.close();
		
		// Close the client
		client.shutdown();
		
		System.out.println("Done!");
	}
	
}
