package com.yahoo.vespa.hosted.node.admin.nodeagent;

import com.yahoo.config.provision.HostName;
import com.yahoo.config.provision.NodeType;
import com.yahoo.vespa.athenz.api.AthenzService;
import com.yahoo.vespa.hosted.dockerapi.ContainerName;
import com.yahoo.vespa.hosted.node.admin.component.TaskContext;
import com.yahoo.vespa.hosted.node.admin.component.ZoneId;
import com.yahoo.vespa.hosted.node.admin.docker.DockerNetworking;

import java.nio.file.Path;
import java.nio.file.Paths;

public interface NodeAgentContext extends TaskContext {

    ContainerName containerName();

    HostName hostname();

    NodeType nodeType();

    AthenzService identity();

    DockerNetworking dockerNetworking();

    ZoneId zone();


    /**
     * This method is the inverse of {@link #pathInNodeFromPathOnHost(Path)}}
     *
     * @param pathInNode absolute path in the container
     * @return the absolute path on host pointing at the same inode
     */
    Path pathOnHostFromPathInNode(Path pathInNode);

    /** @see #pathOnHostFromPathInNode(Path) */
    default Path pathOnHostFromPathInNode(String pathInNode) {
        return pathOnHostFromPathInNode(Paths.get(pathInNode));
    }


    /**
     * This method is the inverse of {@link #pathOnHostFromPathInNode(Path)}
     *
     * @param pathOnHost absolute path on host
     * @return the absolute path in the container pointing at the same inode
     */
    Path pathInNodeFromPathOnHost(Path pathOnHost);

    /** @see #pathOnHostFromPathInNode(Path) */
    default Path pathInNodeFromPathOnHost(String pathOnHost) {
        return pathInNodeFromPathOnHost(Paths.get(pathOnHost));
    }


    /**
     * @param relativePath relative path under Vespa home in container
     * @return the absolute path under Vespa home in the container
     */
    Path pathInNodeUnderVespaHome(Path relativePath);

    /** @see #pathInNodeUnderVespaHome(Path) */
    default Path pathInNodeUnderVespaHome(String relativePath) {
        return pathInNodeUnderVespaHome(Paths.get(relativePath));
    }
}
