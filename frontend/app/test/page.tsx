"use client";

import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardFooter, CardHeader, CardTitle } from "@/components/ui/card";
import { Input } from "@/components/ui/input";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import Navbar from "@/app/UI-Components/Navbar";
import { Box } from "@radix-ui/themes";
import { Send, Server, Shield } from "lucide-react";
import { useState } from "react";

function CyberForm() {
  const [selectedServer, setSelectedServer] = useState("");
  const [message, setMessage] = useState("");
  const [response, setResponse] = useState("");
  const servers = [
    { id: "server1", name: "Server 1 - Fast & Secure" },
    { id: "server2", name: "Server 2 - Ultra Private" },
    { id: "server3", name: "Server 3 - Balanced" },
  ];

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setResponse(`Message sent through ${selectedServer}: "${message}"`);
    setMessage("");
  };
  return (
    <div className="flex-row">
      <Navbar />
      <div className="flex justify-center items-center">
        <Box className="bg-green-1000">
          <section className="min-h-screen flex items-center justify-center">
            <Card className="w-full max-w-md bg-purple-800 border-purple-600">
              <CardHeader>
                <CardTitle className="text-2xl font-bold flex items-center gap-2">
                  <Shield className="w-6 h-6" /> Forward Proxy Server
                </CardTitle>
                <CardDescription className="text-purple-300">Secure and private message forwarding</CardDescription>
              </CardHeader>
              <CardContent>
                <form onSubmit={handleSubmit} className="space-y-4">
                  <div className="space-y-2">
                    <label htmlFor="server-select" className="text-sm font-medium">
                      Select Server
                    </label>
                    <Select onValueChange={setSelectedServer} required>
                      <SelectTrigger id="server-select" className="bg-purple-700 border-purple-600">
                        <SelectValue placeholder="Choose a server" />
                      </SelectTrigger>
                      <SelectContent className="bg-purple-700 border-purple-600">
                        {servers.map((server) => (
                          <SelectItem key={server.id} value={server.id} className="text-white">
                            {server.name}
                          </SelectItem>
                        ))}
                      </SelectContent>
                    </Select>
                  </div>
                  <div className="space-y-2">
                    <label htmlFor="message" className="text-sm font-medium">
                      Your Message
                    </label>
                    <Input
                      id="message"
                      value={message}
                      onChange={(e) => setMessage(e.target.value)}
                      className="bg-purple-700 border-purple-600 text-white placeholder-purple-400"
                      placeholder="Enter your message here"
                      required
                    />
                  </div>
                  <Button type="submit" className="w-full bg-purple-500 hover:bg-purple-400 text-purple-900">
                    <Send className="w-4 h-4 mr-2" /> Send Message
                  </Button>
                </form>
              </CardContent>
              {response && (
                <CardFooter className="bg-purple-700 mt-4 rounded-b-lg">
                  <div className="space-y-2 w-full">
                    <h3 className="font-semibold flex items-center gap-2">
                      <Server className="w-4 h-4" /> Server Response
                    </h3>
                    <p className="text-purple-200 text-sm">{response}</p>
                  </div>
                </CardFooter>
              )}
            </Card>
          </section>
          {/* <Section className="text-center">
            <Text size={"8"}>Cyber Complaint Form</Text>
          </Section> */}

          {/* <Flex direction="column" gap="4">
            <Flex direction={"column"} gap={"1"}>
              Select Category of Crime:
              <Select.Root size={"3"}>
                <Select.Trigger placeholder="Select an option"></Select.Trigger>
                <Select.Content>
                  <Select.Group>
                    <Select.Label>Crime Category</Select.Label>
                    <Select.Item value="1">Day Month Year</Select.Item>
                    <Select.Item value="2">Month Day Year</Select.Item>
                    <Select.Item value="3">Year Day Month</Select.Item>
                    <Select.Item value="4">Year Month Day</Select.Item>
                  </Select.Group>
                </Select.Content>
              </Select.Root>
            </Flex> */}

          {/* <Flex direction={"column"} gap={"1"}>
            Crime Description:
            <TextArea size="3" placeholder="Enter Crime Description..." />
          </Flex>

          <Flex direction={"column"} gap={"1"}>
            Platform:
            <TextField.Root size="3" placeholder="Enter platform on which this occured" />
          </Flex>

          <Flex direction={"column"} gap={"1"}>
            Enter Date:
            <TextField.Root size="3" type="datetime-local" placeholder="Enter Date" />
          </Flex>

          <Flex direction={"column"} gap={"1"}>
            Number of Tokens Staked:
            <TextField.Root size="3" type="number" placeholder="Enter number of tokens you will stake" />
          </Flex>

          <Flex direction={"column"} gap={"1"}>
            Upload Relevant Evidence:
            <Button style={{ width: "8rem" }} size={"2"} color="violet">
              Upload Files
            </Button>
          </Flex>

          <Flex gap="2" className="items-center">
            <Checkbox defaultChecked color="violet" />I certify that the information provided is true and accurate
          </Flex>

          <Button size="3" className="w-full" color="violet">
            Submit
          </Button> */}
        </Box>
      </div>
    </div>
  );
}

export default CyberForm;
