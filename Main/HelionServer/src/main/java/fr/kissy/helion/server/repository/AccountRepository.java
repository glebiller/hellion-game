package fr.kissy.helion.server.repository;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Required;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.stereotype.Repository;
 
/**
 * Repository for {@link fr.kissy.helion.server.domain.Account}s
 *
 */
@Repository
public class AccountRepository {
 
    private static final Logger LOGGER = LoggerFactory.getLogger(AccountRepository.class);
 
    @Autowired
    private MongoTemplate mongoTemplate;

    /**
     * Set mongoTemplate.
     *
     * @param mongoTemplate The mongoTemplate to set.
     */
    @Required
    public void setMongoTemplate(MongoTemplate mongoTemplate) {
        this.mongoTemplate = mongoTemplate;
    }
}