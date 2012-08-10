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
package fr.kissy.hellion.server;

import fr.kissy.hellion.server.config.AppConfig;
import fr.kissy.hellion.server.handler.ServerPipelineFactory;
import org.apache.log4j.BasicConfigurator;
import org.jboss.netty.bootstrap.ServerBootstrap;
import org.jboss.netty.channel.socket.nio.NioServerSocketChannelFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

import java.net.InetSocketAddress;
import java.util.concurrent.Executors;

public class Main {

    private static AnnotationConfigApplicationContext context;

    private final int port;

    public Main(int port) {
        this.port = port;
    }

    public void run() {
        ServerBootstrap bootstrap = new ServerBootstrap(
                new NioServerSocketChannelFactory(Executors.newCachedThreadPool(), Executors.newCachedThreadPool())
        );
        bootstrap.setPipelineFactory(new ServerPipelineFactory());
        bootstrap.bind(new InetSocketAddress(port));
    }

	public static void main(String[] args) {
        context = new AnnotationConfigApplicationContext();
        context.register(AppConfig.class);
        context.scan("fr.kissy.hellion.server.repository");
        context.refresh();
        BasicConfigurator.configure();

        new Main(8080).run();
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