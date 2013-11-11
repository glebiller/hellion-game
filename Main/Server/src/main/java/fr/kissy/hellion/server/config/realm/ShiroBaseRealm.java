package fr.kissy.hellion.server.config.realm;

import fr.kissy.hellion.server.domain.Account;
import fr.kissy.hellion.server.repository.AccountRepository;
import org.apache.shiro.authc.AuthenticationException;
import org.apache.shiro.authc.AuthenticationInfo;
import org.apache.shiro.authc.AuthenticationToken;
import org.apache.shiro.authc.SimpleAuthenticationInfo;
import org.apache.shiro.authc.credential.CredentialsMatcher;
import org.apache.shiro.authc.credential.HashedCredentialsMatcher;
import org.apache.shiro.authz.AuthorizationException;
import org.apache.shiro.authz.AuthorizationInfo;
import org.apache.shiro.authz.SimpleAuthorizationInfo;
import org.apache.shiro.crypto.hash.Sha1Hash;
import org.apache.shiro.realm.AuthorizingRealm;
import org.apache.shiro.subject.PrincipalCollection;
import org.apache.shiro.util.ByteSource;
import org.apache.shiro.util.SimpleByteSource;
import org.springframework.beans.factory.annotation.Autowired;

public class ShiroBaseRealm extends AuthorizingRealm {

    @Autowired
    private AccountRepository accountRepository;
    private HashedCredentialsMatcher matcher;

    /**
     * Constructor
     */
    public ShiroBaseRealm() {
        matcher = new HashedCredentialsMatcher();
        matcher.setHashAlgorithmName(Sha1Hash.ALGORITHM_NAME);
    }

    /**
     * @inheritDoc
     */
    @Override
    public CredentialsMatcher getCredentialsMatcher() {
        return matcher;
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean supports(AuthenticationToken token) {
        return true;
    }

    /**
     * @inheritDoc
     */
    @Override
    protected AuthenticationInfo doGetAuthenticationInfo(AuthenticationToken token) throws AuthenticationException {
        Account account = accountRepository.findByUsername(token.getPrincipal().toString());
        if (account == null) {
            throw new AuthenticationException();
        }

        ByteSource salt = new SimpleByteSource(String.valueOf(token.getPrincipal()));
        return new SimpleAuthenticationInfo(account.getUsername(), account.getPassword(), salt, this.getName());
    }



    /**
     * @inheritDoc
     */
    @Override
    @SuppressWarnings("unchecked")
    protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principals) {
        if (principals == null) {
            throw new AuthorizationException("PrincipalCollection method argument cannot be null.");
        }
        return new SimpleAuthorizationInfo();
    }

}