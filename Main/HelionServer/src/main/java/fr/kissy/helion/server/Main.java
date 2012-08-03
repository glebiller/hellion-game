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
package fr.kissy.helion.server;

import fr.kissy.helion.server.config.AppConfig;
import fr.kissy.helion.server.proto.service.Authentification;
import fr.kissy.helion.server.rpc.NettyRpcServer;
import fr.kissy.helion.server.service.AuthentificationServiceImpl;
import org.apache.log4j.BasicConfigurator;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

public class Main {

    private static ApplicationContext context;

	public static void main(String[] args) {
        // Init spring
        context = new AnnotationConfigApplicationContext(AppConfig.class);

        // Configure the netty server
		BasicConfigurator.configure();
		NettyRpcServer server = new NettyRpcServer(
				new NioServerSocketChannelFactory(
						Executors.newCachedThreadPool(), 
						Executors.newCachedThreadPool()
                )
        );

        // Register services
		server.registerService(Authentification.AuthentificationService.newReflectiveService(new AuthentificationServiceImpl()));
		server.registerBlockingService(Authentification.AuthentificationService.newReflectiveBlockingService(new AuthentificationServiceImpl()));

        // Launch the server
		server.serve(new InetSocketAddress(8080));
	}

    /**
     * Get the spring context.
     *
     * @return The spring context.
     */
    public static ApplicationContext getContext() {
        return context;
    }
}
