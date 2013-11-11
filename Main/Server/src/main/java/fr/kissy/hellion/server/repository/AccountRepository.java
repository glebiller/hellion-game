package fr.kissy.hellion.server.repository;

import fr.kissy.hellion.server.domain.Account;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.MongoOperations;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.data.mongodb.repository.query.MongoEntityInformation;
import org.springframework.data.mongodb.repository.support.MongoRepositoryFactory;
import org.springframework.data.mongodb.repository.support.SimpleMongoRepository;
import org.springframework.stereotype.Repository;
 
/**
 * Repository for {@link fr.kissy.hellion.server.domain.Account}s
 */
@Repository
public class AccountRepository extends SimpleMongoRepository<Account, String> {
    private static final Logger LOGGER = LoggerFactory.getLogger(AccountRepository.class);

    /**
     * Default constructor.
     */
    public AccountRepository(MongoEntityInformation<Account, String> metadata, MongoOperations mongoOperations) {
        super(metadata, mongoOperations);
    }

    /**
     * @inheritDoc
     */
    @Autowired
    public AccountRepository(MongoRepositoryFactory factory, MongoOperations mongoOperations) {
        this(factory.<Account, String>getEntityInformation(Account.class), mongoOperations);
    }

    /**
     * Find by username.
     *
     * @param username The username.
     * @return The found account.
     */
    public Account findByUsername(String username) {
        return getMongoOperations().findOne(new Query(Criteria.where("username").is(username)), Account.class,
                getEntityInformation().getCollectionName());
    }

}