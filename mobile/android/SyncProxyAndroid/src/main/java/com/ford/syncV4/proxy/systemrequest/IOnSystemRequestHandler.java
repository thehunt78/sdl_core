package com.ford.syncV4.proxy.systemrequest;

import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.RequestType;

import java.util.List;

/**
 * Interface for handling special OnSystemRequest notifications.
 *
 * Created by enikolsky on 2014-02-03.
 */
public interface IOnSystemRequestHandler {
    /**
     * Requests to download the files from the given URLs. When done, each of
     * the files should be uploaded with the {@link ISystemRequestProxy#putSystemFile(java.lang.String,
     * String, byte[], Integer, com.ford.syncV4.proxy.rpc.enums.FileType)} method.
     *
     * @param proxy    the proxy used to upload the downloaded files
     * @param urls     a list of URLs to download files from
     * @param fileType optional file type for HTTP requests
     */
    public void onFilesDownloadRequest(String appId, ISystemRequestProxy proxy,
                                       List<String> urls, FileType fileType);

    /**
     * Requests the rest of the given file ([offset … offset+length] bytes).
     * When done, the data should be uploaded with the {@link
     * ISystemRequestProxy#putSystemFile(java.lang.String, String, byte[],
     * com.ford.syncV4.proxy.rpc.enums.FileType)}
     * method.
     *
     * @param proxy    the proxy used to upload the given file
     * @param filename the requested file's name
     * @param offset   offset of the requested data
     * @param length   length of the requested data
     * @param fileType optional file type
     */
    public void onFileResumeRequest(String appId, ISystemRequestProxy proxy, String filename,
                                    Integer offset, Integer length,
                                    FileType fileType);

    /**
     * Requests to get Policy Table Snapshot file as raw data and process it. When done, result
     * should be upload with
     *
     * @param proxy The proxy used to upload the given file
     * @param data  Raw bytes data of the Policy Table Snapshot file
     * @param fileType type of the file
     * @param requestType type of the request
     */
    public void onPolicyTableSnapshotRequest(String appId, final ISystemRequestProxy proxy,
                                             byte[] data, FileType fileType, RequestType requestType);
}
