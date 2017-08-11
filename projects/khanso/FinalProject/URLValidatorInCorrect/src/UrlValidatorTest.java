/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import com.sun.tools.doclets.formats.html.SourceToHTMLConverter;
import junit.framework.TestCase;
import org.junit.Ignore;
import org.junit.Test;

import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringJoiner;
import java.util.stream.Collectors;


/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

    private boolean printStatus = false;
    private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

    public UrlValidatorTest(String testName) {
        super(testName);
    }


    static final int SCHEME = 0;
    static final int HOST = 1;
    static final int PORT = 2;
    static final int PATH = 3;
    static final int QUERY_STRING = 4;
    static final int FRAGMENT = 5;

    static final String[] validStd = {"http://", "www.google.com", "", "/somefile.csv", "", ""};
    static final String[] invalidStd = {"ffa/??://", "zzz?", ":32ze", "/wrong /Path", " ? ", "#wrong fragment"};


    public void testManualTest()
    {
        UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_LOCAL_URLS + UrlValidator.ALLOW_2_SLASHES);

        // test null url
        System.out.println("Check null url");
        System.out.println(!urlVal.isValid(null));
        System.out.println("Check empty string");
        System.out.println(!urlVal.isValid(""));

        // Check basic schemes
        System.out.println("Check basic schemes without port.");
        System.out.println(urlVal.isValid("http://www.amazon.com") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com") == true);
        System.out.println(urlVal.isValid("ftp://www.amazon.com") == true);
        System.out.println(urlVal.isValid("http://www.amazon.com/homepage.html") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com/homepage.html") == true);
        System.out.println(urlVal.isValid("ftp://www.amazon.com/homepage.html") == true);

        // Check ports
        System.out.println("\nCheck basic schemes with ports.");
        System.out.println(urlVal.isValid("http://www.amazon.com:") == false);
        System.out.println(urlVal.isValid("https://www.amazon.com:") == false);
        System.out.println(urlVal.isValid("ftp://www.amazon.com:") == false);
        System.out.println(urlVal.isValid("http://www.amazon.com:1") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com:22") == true);
        System.out.println(urlVal.isValid("ftp://www.amazon.com:333") == true);
        System.out.println(urlVal.isValid("http://www.amazon.com:4444") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com:55555") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com:65535") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com:65536") == false);
        System.out.println(urlVal.isValid("ftp://www.amazon.com:666666") == false);

        // Check queries
        System.out.println("\nCheck queries.");
        System.out.println(urlVal.isValid("https://www.amazon.com:12?action=view") == true);
        System.out.println(urlVal.isValid("https://www.amazon.com:65?action=view") == true);
        System.out.println(urlVal.isValid("ftp://www.amazon.com:45?action") == true);
        System.out.println(urlVal.isValid("http://www.google.com?action=view") == true);

        // Check local host
        System.out.println("\nCheck local hosts.");
        System.out.println(urlVal.isValid("http://localhost/") == true);
        System.out.println(urlVal.isValid("http://machine/") == true);
        System.out.println(urlVal.isValid("http://127.0.0.1/") == true);

        // Check fragments
        System.out.println("\nCheck fragments.");
        System.out.println(urlVal.isValid("http://www.somesite.org:33/somepage.csv#row=23") == true);

        // Check random stuff
        System.out.println("\nCheck random stuff.");
        System.out.println(urlVal.isValid("http://amazon.com/") == true);
        System.out.println(urlVal.isValid("htttp://www.amazon.com/") == false);
        System.out.println(urlVal.isValid("dfssfsdsdfsdf://www.amazon.com/") == false);
    }

    void testHost(UrlValidator v, String host, boolean expected){
        String url = validStd[SCHEME] + host + validStd[PORT] + validStd[PATH] + validStd[QUERY_STRING] + validStd[FRAGMENT];
        boolean result = v.isValid(url);
        _assertEquals("Host Test: " + host + " URL: " + url, expected, result);
        url = invalidStd[SCHEME] + host;
        result = v.isValid(url);
        _assertEquals("Host Test: " + host + " URL: " + url, false, result);
    }

    public void testUrlValidatorValidatesHost() {
        System.out.println("Host Partition Tests");
        UrlValidator validator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
        String[] validNumberTests = {"255.255.255.255", "0.0.0.0", "4.3.2.1", "01.001.000.009", "10.10.10.10"};
        String[] invalidNumberTests = {"-1.-10.-100.-256", "256.256.256.256",  "1111.11111.1.1"};
        String[] invalidAlphaNumTests = {"1.a.1.1", "a.a.a.a", "1.!.1.1", "1a.1.1.1", "a1.1.1.1"};
        String[] invalidIPStructureTests = {"1..1.1.1", ".1.1.1.1", "1.1.1.1.", "1.1.1.1.1", "1.1.1", "....", "1,1.1.1"};

        String[] validHostnameTests = {"www.google.com", "w2w.google.com", "google.co.uk", "google.ca", "www.google.com.br", "www.goo.gl.com", "221.google.com"};
        String[] invalidHostnameTests = { "www.google", ".www.google.com", "www,google.com", "www.google.", ""};

        String testCase[][] = {validNumberTests, validHostnameTests, invalidNumberTests, invalidAlphaNumTests, invalidIPStructureTests, invalidHostnameTests};
        String host;
        boolean expected;

        for (int i = 0; i < testCase.length; i++){
            for (int j =0; j < testCase[i].length; j++){
                expected = i < 2;
                host = testCase[i][j];
                testHost(validator, host, expected);
            }
        }
    }

    void testScheme(UrlValidator v, String scheme, boolean expected){
        String url = scheme + validStd[HOST] + validStd[PORT] + validStd[PATH] + validStd[QUERY_STRING] + validStd[FRAGMENT];
        boolean result = v.isValid(url);
        _assertEquals("Scheme Test: " + scheme + " URL: " + url, expected, result);
        url = scheme + invalidStd[HOST];
        result = v.isValid(url);
        _assertEquals("Scheme Test: " + scheme + " URL: " + url, false, result);
    }

    public void testUrlValidatorValidatesScheme() {
        System.out.println("Scheme Partition Tests");
        UrlValidator validator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
        String scheme;
        String testUrl;
        boolean expected;
        boolean result;

        String validBefore[] = {"http", "ftp", "ft2p", "https", "irc", "redis", "ut2004", "wss"};
        String invalidBefore[] = {"", "3nc", "..", "/htpp", "12", "!http"};
        String before[][] = {validBefore, invalidBefore};
        String suffix[] = {"//", "/", ""};
        String semiColon [] = {":", ""};

        // test combinations of valid and invalid sections of the scheme
        for (int beforeArray = 0; beforeArray < before.length; beforeArray++ ){
            for (int beforeIndex = 0; beforeIndex < before[beforeArray].length; beforeIndex++){
                for (int semiColonIndex = 0; semiColonIndex < semiColon.length; semiColonIndex++){
                    for (int suffixIndex = 0; suffixIndex < suffix.length; suffixIndex++){
                        scheme = before[beforeArray][beforeIndex] + semiColon[semiColonIndex] + suffix[suffixIndex];
                        expected = (beforeArray == 0 && semiColonIndex == 0 && suffixIndex == 0);
                        testScheme(validator, scheme, expected);
                    }
                }
            }
        }

        // test out of order scheme sections that would be valid in order
        String orderTests[] = {"//:http", "http//:", "://http", ":http//"};
        for (int i = 0; i < orderTests.length; i++){
            testScheme(validator, orderTests[i],false);
        }
    }

    class Case {
        private String message;
        private String testCase;
        private boolean validity;

        public Case (String _message, String _testCase, boolean _validity) {
            message = _message;
            testCase = _testCase;
            validity = _validity;
        }
    }

    private static int _assertEquals(String message, boolean expected, boolean actual) {
        if (expected != actual) {
            System.out.println("Failed: " + message + " Expected: " + expected);
            return 1;
        }
        return 0;
    }

    private void _runThroughCases(Case[] cases, UrlValidator validator, int componentUnderTest) {
        String[] validParts = validStd.clone();
        String[] invalidParts = invalidStd.clone();
        String url;
        boolean result;

        int totalTests = 0;
        int failingTests = 0;
        for (Case _case: cases) {
            validParts[componentUnderTest] = _case.testCase;
            invalidParts[componentUnderTest] = _case.testCase;

            url = Arrays.stream(validParts).collect(Collectors.joining(""));
            result = validator.isValid(url);
            failingTests += _assertEquals(_case.message + " URL: " + url, _case.validity, result);
            totalTests++;

            url = Arrays.stream(invalidParts).collect(Collectors.joining(""));
            result = validator.isValid(url);
            failingTests += _assertEquals(_case.message + " URL: " + url, false, result);  // should be false regardless
            totalTests++;
        }

        System.out.println("Total tests: " + totalTests + " Failing Tests: " + failingTests);
    }
    /**
     * partition the ports and test them with control urls
     */
    public void testUrlValidatorValidatesPorts()
    {
        System.out.println("Port Partition Tests");
        Case[] cases = {
                new Case("min port", ":1", true),
                new Case("zero port", ":0", true),
                new Case("negative port not allowed", ":-1", false),
                new Case("http port is good", ":80", true),
                new Case("well known python app port should be good", ":8888", true),
                new Case("runescape port should be good", ":43594", true),
                new Case("maximum port", ":65535", true),
                new Case("larger than maximum", ":65536", false),
                new Case("port has foreign characters", ":45ZXZ", false),
                new Case("all letters not allowed as well", ":ABCDEFG", false),
                new Case("port has spaces", ": 3423", false),
                new Case("port is empty, signifies default port", "", true)

        };


        UrlValidator validator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

        _runThroughCases(cases, validator, PORT);
    }

    public void testValidatorValidatesPathsAndOptions()
    {
        System.out.println("Paths and Options Partition Test");
        Case[] cases = {
                new Case("root path is okay",  "/", true),
                new Case("top level path is okay", "/purchases", true),
                new Case("even longer path is fine", "/purchases/by-date/tomorrow", true),
                new Case("path with a space prepended is wrong", " /", false),
                new Case("path with space at end is incorrect", "/path/path/ ", false),
                new Case("path with incorrect percent encoding incorrect", "/path/path%2space", false),
                new Case("path with correct percent encoding is correct", "/path/path%20space", true)
        };

        UrlValidator validator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
        _runThroughCases(cases, validator, PATH);
    }

/*    public void testValidatorValidatesFragments()
    {
        Case[] cases = {
                new Case("fragment is empty", "#", true),
                new Case("fragment has an anchor", "#profile", true),
                new Case("fragment contains a space before pound sign", " #", false),
                new Case("fragment is long and has special characters", "#a-really-long-fragment", true),
                new Case("fragment is not encoded correctly", "# space-should-be-encoded", false),
                new Case("fragment contains encoded space", "#%20-space-should-be-encoded", true),
                new Case("fragment contains invalid encoding", "#%zz-not-valid-encoding", false)
        };

        UrlValidator validator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
        _runThroughCases(cases, validator, FRAGMENT);
    }

*/

    // Helper to check the result and print
    private boolean checkAndPrint(UrlValidator val, String url, boolean expected)
    {
        if(expected == true)
        {
            if(val.isValid(url) == expected)
            {
                System.out.println(url + " CORRECTLY passes."); return true;
            }
            else
            {
                System.out.println(url + " INCORRECTLY fails.");	return false;
            }
        }
        else
        {
            if(val.isValid(url) == expected)
            {
                System.out.println(url + " CORRECTLY fails."); return true;
            }
            else
            {
                System.out.println(url + " INCORRECTLY passes."); return false;
            }
        }
    }


    // Partition testing
    public void testFragmentPartition()
    {
        // Tell the user whats happening
        System.out.println("\nTesting Fragments (Section 2)");

        // Spin up the validator object
        UrlValidator urlVal = new UrlValidator();

        // Input partition cases for query
        String[] inputFragmentPartsPass = {"#row=123", "#row=asda", "#row=123,128", "#row=123-125", "#row=123,128&col=13212"};
        String[] inputFragmentPartsFail = {"row=123", "#rowasda", "#row=123#128", "row=123125", "#row=123,128col=13212"};

        // Contruct pre and post for clean code in loops
        String preFragmentPass = validStd[SCHEME] + validStd[HOST] + validStd[PORT] + validStd[PATH];
        String postFragmentPass = validStd[FRAGMENT];
        String preFragmentFail = invalidStd[SCHEME] + invalidStd[HOST] + invalidStd[PORT] + invalidStd[PATH];
        String postFragmentFail = invalidStd[FRAGMENT];

        // Test result counters
        int passCount = 0; int failCount = 0;

        // Construct URL's for true section under test and true control
        for(String pass : inputFragmentPartsPass)
        {
            String url = preFragmentPass + pass + postFragmentPass;
            if(checkAndPrint(urlVal, url, true)) passCount ++; else failCount ++;
        }

        // Construct URL's for true section under test and false control
        for(String pass : inputFragmentPartsPass)
        {
            String url = preFragmentFail + pass + postFragmentFail;
            if(checkAndPrint(urlVal, url, false)) passCount ++; else failCount ++;
        }

        // Construct URL's for false section under test and true control
        for(String fail : inputFragmentPartsFail)
        {
            String url = preFragmentPass + fail + postFragmentPass;
            if(checkAndPrint(urlVal, url, false)) passCount ++; else failCount ++;
        }

        // Construct URL's for false section under test and false control
        for(String fail : inputFragmentPartsFail)
        {
            String url = preFragmentFail + fail + postFragmentFail;
            if(checkAndPrint(urlVal, url, false)) passCount ++; else failCount ++;
        }

        System.out.println("\nTest fragments final results (Section 3) pass count = " +
                Integer.toString(passCount) + " and fail count = " + Integer.toString(failCount) + ".");
    }

    public void testQueryPartition()
    {
        // Tell the user whats happening
        System.out.println("\nTesting queries (Section 2)");

        // Spin up the validator object
        UrlValidator urlVal = new UrlValidator();

        // Input partition cases for query
        String[] inputQueryPartsPass = {"?pid=21", "?pid=dsdffs", "?pid=21&risc=98", "?pid=21&risc=98&risc=98"};
        String[] inputQueryPartsFail = {"pid=34", "?pid?dsdffs", "?pid=21risc=98", "?pid=21risc=98&risc=98"};

        // Contruct pre and post for clean code in loops
        String preQueryPass = validStd[SCHEME] + validStd[HOST] + validStd[PORT] + validStd[PATH];
        String postQueryPass = validStd[FRAGMENT];
        String preQueryFail = invalidStd[SCHEME] + invalidStd[HOST] + invalidStd[PORT] + invalidStd[PATH];
        String postQueryFail = invalidStd[FRAGMENT];

        // Test result counters
        int passCount = 0; int failCount = 0;

        // Construct URL's for true section under test and true control
        for(String pass : inputQueryPartsPass)
        {
            String url = preQueryPass + pass + postQueryPass;
            if(checkAndPrint(urlVal, url, true)) passCount ++; else failCount ++;
        }

        // Construct URL's for true section under test and false control
        for(String pass : inputQueryPartsPass)
        {
            String url = preQueryFail + pass + postQueryFail;
            if(checkAndPrint(urlVal, url, false)) passCount ++; else failCount ++;
        }

        // Construct URL's for false section under test and true control
        for(String fail : inputQueryPartsFail)
        {
            String url = preQueryPass + fail + postQueryPass;
            if(checkAndPrint(urlVal, url, false)) passCount ++; else failCount ++;
        }

        // Construct URL's for false section under test and false control
        for(String fail : inputQueryPartsFail)
        {
            String url = preQueryFail + fail + postQueryFail;
            if(checkAndPrint(urlVal, url, false)) passCount ++; else failCount ++;
        }

        System.out.println("\nTest queries final results (Section 2) pass count = " +
                Integer.toString(passCount) + " and fail count = " + Integer.toString(failCount) + ".");
    }

    private Case[] generateSchemes() {
        String validBefore[] = {"http", "ftp", "ft2p", "https", "irc", "redis", "ut2004", "wss"};
        String invalidBefore[] = {"", "3nc", "..", "/htpp", "12", "!http"};
        String beforeSchemes[][] = {validBefore, invalidBefore};
        String suffix[] = {"//", "/", ""};
        String semiColon [] = {":", ""};
        String scheme = "";
        String msg = "";
        boolean expected;
        ArrayList<Case> cases = new ArrayList<>();

        for (int beforeSchemesArray = 0; beforeSchemesArray < beforeSchemes.length; beforeSchemesArray++ ) {
            for (int beforeSchemesIndex = 0; beforeSchemesIndex < beforeSchemes[beforeSchemesArray].length; beforeSchemesIndex++) {
                for (int semiColonIndex = 0; semiColonIndex < semiColon.length; semiColonIndex++) {
                    for (int suffixIndex = 0; suffixIndex < suffix.length; suffixIndex++) {
                        scheme = beforeSchemes[beforeSchemesArray][beforeSchemesIndex] + semiColon[semiColonIndex] + suffix[suffixIndex];
                        expected = (beforeSchemesArray == 0 && semiColonIndex == 0 && suffixIndex == 0);
                        msg = scheme + " should " + (expected ? " pass": " fail");
                        cases.add(new Case(msg, scheme, expected));
                    }
                }
            }
        }

        return cases.toArray(new Case[cases.size()]);
    }

    private Case[] generateQueryStringCases()
    {
        String[] inputQueryPartsPass = {"?pid=21", "?pid=dsdffs", "?pid=21&risc=98", "?pid=21&risc=98&risc=98"};
        String[] inputQueryPartsFail = {"pid=34", "?pid?dsdffs", "?pid=21risc=98", "?pid=21risc=98&risc=98"};

        ArrayList<Case> cases = new ArrayList<>();

        for (int i = 0; i < inputQueryPartsPass.length; i++) {
            cases.add(new Case("Query " + inputQueryPartsPass[i] + " should pass", inputQueryPartsPass[i], true));
        }

        for (int i = 0; i < inputQueryPartsPass.length; i++) {
            cases.add(new Case("Query " + inputQueryPartsFail[i] + "should fail", inputQueryPartsFail[i], true));
        }

       return cases.toArray(new Case[cases.size()]);
    }

    private Case[] generateHostCases()
    {
        String[] validNumberTests = {"255.255.255.255", "0.0.0.0", "4.3.2.1", "01.001.000.009", "10.10.10.10"};
        String[] invalidNumberTests = {"-1.-10.-100.-256", "256.256.256.256",  "1111.11111.1.1"};
        String[] invalidAlphaNumTests = {"1.a.1.1", "a.a.a.a", "1.!.1.1", "1a.1.1.1", "a1.1.1.1"};
        String[] invalidIPStructureTests = {"1..1.1.1", ".1.1.1.1", "1.1.1.1.", "1.1.1.1.1", "1.1.1", "....", "1,1.1.1"};

        String[] validHostnameTests = {"www.google.com", "w2w.google.com", "google.co.uk", "google.ca", "www.google.com.br", "www.goo.gl.com", "221.google.com"};
        String[] invalidHostnameTests = { "www.google", ".www.google.com", "www,google.com", "www.google.",  ""};
        String hostTestCase[][] = {validNumberTests, validHostnameTests, invalidNumberTests, invalidAlphaNumTests, invalidIPStructureTests, invalidHostnameTests};

        ArrayList<Case> cases = new ArrayList<>();
        String msg = "";
        String host;
        boolean expected;
        for (int i = 0; i < hostTestCase.length; i++) {
            expected =  i < 2;
            if (i == 1 || i == 5) {
                msg = "Hostname test ";
            } else {
                msg = "Dotted decimal test ";
            }
            for (int j = 0; j < hostTestCase[i].length; j++) {
                host = hostTestCase[i][j];
                cases.add(new Case(msg + host + " should " + (expected? "pass": "fail"), host, expected));
            }
        }

        return cases.toArray(new Case[cases.size()]);
    }

    private Case[] generateFragmentCases()
    {
        String[] inputFragmentPartsPass = {"#row=123", "#row=asda", "#row=123,128", "#row=123-125", "#row=123,128&col=13212"};
        String[] inputFragmentPartsFail = {"row=123", "#rowasda", "#row=123#128", "row=123125", "#row=123,128col=13212"};

        ArrayList<Case> cases = new ArrayList<>();

        for (String s: inputFragmentPartsPass) {
            cases.add(new Case("Fragment: " + s + " should pass", s, true));
        }

        for (String s: inputFragmentPartsFail) {
            cases.add(new Case("Fragment: " + s +  " should not pass", s, false));
        }
        return cases.toArray(new Case[cases.size()]);
    }

    private Case[] generatePortCases()
    {

        Case[] portCases = {
                new Case("min port", ":1", true),
                new Case("zero port", ":0", false),
                new Case("negative port not allowed", ":-1", false),
                new Case("http port is good", ":80", true),
                new Case("well known python app port should be good", ":8888", true),
                new Case("runescape port should be good", ":43594", true),
                new Case("maximum port", ":65535", true),
                new Case("larger than maximum", ":65536", false),
                new Case("port has foreign characters", ":45ZXZ", false),
                new Case("all letters not allowed as well", ":ABCDEFG", false),
                new Case("port has spaces", ": 3423", false),
                new Case("port is empty, signifies default port", "", true)

        };

        return portCases;
    }

    private Case[] generatePathCases() {
        Case[] pathCases = {
                new Case("root path is okay",  "/", true),
                new Case("top level path is okay", "/purchases", true),
                new Case("even longer path is fine", "/purchases/by-date/tomorrow", true),
                new Case("path with a space prepended is wrong", " /", false),
                new Case("path with space at end is incorrect", "/path/path/ ", false),
                new Case("path with incorrect percent encoding incorrect", "/path/path%2space", false),
                new Case("path with correct percent encoding is correct", "/path/path%20space", true)
        };

        return pathCases;
    }

    public static int testCombo(UrlValidator validator, List<String> parts, boolean[] validityOfComponents) {
        boolean expected = true;
        boolean result;
        for (boolean b: validityOfComponents) {
            expected &= b;
        }
        String url = parts.stream().collect(Collectors.joining(""));
        result = validator.isValid(url);
        if (result != expected) {
            System.out.println("FAILURE: " + url + " EXPECTED: " + expected);
            return 1;
        }
        return 0;
    }

    public void testCombos()
    {
        boolean expected = true;
        List<String> urlParts = Arrays.asList(validStd); // initialize with valid components for place fillers
        boolean[] validityOfComponents = new boolean[validStd.length];
        UrlValidator validator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
        String scheme = "";

        Case[] schemeCases = generateSchemes();
        Case[] queryCases =  generateQueryStringCases();
        Case[] fragmentCases =  generateFragmentCases();
        Case[] hostCases =  generateHostCases();
        Case[] portCases = generatePortCases();
        Case[] pathCases = generatePathCases();

        int totalTestCases = 0;
        int failedTestCases = 0;
        for (Case schemeCase: schemeCases) {
            scheme = schemeCase.testCase;
            validityOfComponents[SCHEME] = schemeCase.validity;
            urlParts.set(SCHEME, scheme);
            for (Case hostCase: hostCases ) {
                validityOfComponents[HOST] = hostCase.validity;
                urlParts.set(HOST, hostCase.testCase);
                for (Case portCase : portCases) {
                    validityOfComponents[PORT] = portCase.validity;
                    urlParts.set(PORT, portCase.testCase);
                    for (Case pathCase : pathCases) {
                        validityOfComponents[PATH] = pathCase.validity;
                        urlParts.set(PATH, pathCase.testCase);
                        for (Case queryCase : queryCases) {
                            validityOfComponents[QUERY_STRING] = queryCase.validity;
                            urlParts.set(QUERY_STRING, queryCase.testCase);
                            for (Case fragmentCase : fragmentCases) {
                                validityOfComponents[FRAGMENT] = fragmentCase.validity;
                                urlParts.set(FRAGMENT, fragmentCase.testCase);
                                failedTestCases += testCombo(validator, urlParts, validityOfComponents);
                                totalTestCases++;
                            }
                        }
                    }
                }
            }
        }
        System.out.println("SUMMARY");
        System.out.println("TOTAL TEST CASES: " + totalTestCases);
        System.out.println("FAILED TEST CASES: " + failedTestCases);
    }

}
