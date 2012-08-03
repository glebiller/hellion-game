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
package fr.kissy.helion.server.service;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.google.protobuf.ServiceException;
import fr.kissy.helion.server.proto.service.Authentification;
import org.apache.log4j.Logger;
import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.subject.Subject;

public class AuthentificationServiceImpl implements Authentification.AuthentificationService.Interface, Authentification.AuthentificationService.BlockingInterface {

	private static final Logger logger = Logger.getLogger(AuthentificationServiceImpl.class);

    @Override
    public Authentification.LogonResponse login(RpcController controller, Authentification.LogonRequest request) throws ServiceException {
        UsernamePasswordToken token = new UsernamePasswordToken(request.getUsername(), request.getPassword());
        Subject currentUser = SecurityUtils.getSubject();
        try {
            currentUser.login(token);
        } catch (Exception e) {
            return Authentification.LogonResponse.newBuilder()
                    .setStatus(Authentification.LogonResponseStatus.FAILURE).build();
        }
        return Authentification.LogonResponse.newBuilder()
                .setStatus(Authentification.LogonResponseStatus.SUCCESS).build();
    }

    @Override
    public void login(RpcController controller, Authentification.LogonRequest request, RpcCallback<Authentification.LogonResponse> done) {
        Authentification.LogonResponse response = null;
        try {
            response = login(controller, request);
        } catch (ServiceException ignored) {}
        if (done != null) {
            done.run(response);
        } else {
            if (response == null) {
                logger.info("Error occured");
            } else {
                logger.info("Login status is " + response.getStatus());
            }
        }
    }
}
